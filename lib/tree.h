#pragma once
#include <stdio.h>
#include <stdlib.h>
#include "flog.h"

#ifndef NDEBUG
#define dump(clas) clas.dumpInside (#clas, __FILE__, __FUNCTION__, __LINE__) ///< NEW_STRUCT dump macros
#else
#define dump(clas) ;
#endif

enum NodType {

    BLANK    = 1,
    CONSTANT = 2,
    X        = 3,
    PLUS     = 4,
    MINUS    = 5,
    MULT     = 6,
    DIV      = 7,
    POWER    = 8
};

struct Nod {

    Nod*    prev    = NULL;
    NodType type    = BLANK;
    double  val     = 0;
    int     NodNum  = 0;
    Nod*    left    = NULL;
    Nod*    right   = NULL;
};

template<typename ELEM_T>
class Tree {

    //constants
    static constexpr unsigned int       CANL      = 0xDEADBEEF; ///< Left cannary of a structure
    static constexpr unsigned int       CANR      = 0xD34DB33F; ///< Right cannary of a structure
    static constexpr unsigned char      POISON1   = 0xBD; ///< 1 byte Poison
    static constexpr unsigned short     POISON2   = 0xBDCF; ///< 2 byte Poison
    static constexpr unsigned int       POISON4   = 0xBADC0FEE; ///< 4 byte Poison
    static constexpr unsigned long long POISON8   = 0xBADC0FEEF04DED32; ///< 8 byte Poison
    static constexpr unsigned int       MULT      = 37u; ///< Multiplier for hash
    static constexpr unsigned int       MAX_RANKS = 100;
    /// @brief Describes errors within NEW_STRUCT
    enum errorCodes {
        ok                   = 0,    ///< All ok
        POISON_ACCESS        = 1<<0, ///< One or more struct elements is poison
        BAD_CAN_L            = 1<<1, ///< Dead left cannary of structure
        BAD_CAN_R            = 1<<2, ///< Dead right cannary of structure
        BAD_data_CAN_L       = 1<<3, ///< Dead left cannary of  data
        BAD_data_CAN_R       = 1<<4, ///< Dead right cannary of data
        NULL_data_PTR        = 1<<5, ///< NULL ptr for data
        NULL_data_CAN_L_PTR  = 1<<6, ///< NULL ptr for left  data cannary
        NULL_data_CAN_R_PTR  = 1<<7, ///< NULL ptr for right data cannary
        WRONG_SIZE           = 1<<8, ///< Something wrong with size var
        POISONED_ERRCOD      = 1<<9, ///< Errcod variable is poisoned; Ususally means that struct has been destructed
        WRONG_HASH           = 1<<10 ///< Hash was changed without any changes from specified function
    };

    //Not constants actually

    unsigned int   canL      = CANL; ///< left cannary of struct
    unsigned int   hash      = 0;    ///< hash value
    size_t         errCode   = ok;   ///< error code
    ELEM_T*        data      = NULL; ///< Ptr to data
    unsigned int*  dataCanL  = NULL; ///< left cannary of data
    unsigned int*  dataCanR  = NULL; ///< right cannary of data
    unsigned int   canR      = CANR; ///< right cannary of struct

    public:

    ELEM_T* getdata () {

        return data;
    }

    void NULLdata () {
        data = NULL;
    }

    unsigned int geterrCode () {
        return errCode;
    }

    Tree () {

        canL      = CANL;
        canR      = CANR;
        hash      = 0;

        dataCanL = (unsigned int*) calloc (sizeof (ELEM_T) + 2 * sizeof (unsigned int), 1);
        assert (dataCanL != NULL);
        data = NodCtor (NULL, BLANK, 0, NULL, NULL, (ELEM_T*) (dataCanL + 1));
        assert (data != NULL);
        dataCanR = (unsigned int*) (data + 1);

        errCode   = ok;
       *dataCanL = CANL;
       *dataCanR = CANR;

        countHash();
    }

    void countHashSeg (char* lft, char* rgt, unsigned int* mult) {

        for (; lft < rgt; lft++) {

            hash += ((unsigned int) *lft) * (*mult);
            *mult *= MULT;
        }
    }

    unsigned int countHash () {

        unsigned int multiplier = 1;
        hash = 0;

        countHashSeg ((char*) &canL, (char*) &canR, &multiplier);

        if (dataCanL != NULL) countHashSeg ((char*) dataCanL, (char*) dataCanR, &multiplier);

        DataCountHash (&multiplier);

        return hash;
    }

    bool verifyHash () {

        if (hash != countHash ()) {

            errCode |= WRONG_HASH;
            return false;
        }
        return true;
    }

    template<typename varType>
    void setPoison (varType* var) {

        if (var == NULL) return;

        switch(sizeof (varType)) {
            case 1 : *((unsigned char*     ) var)      = POISON1; break;
            case 2 : *((unsigned short*    ) var)      = POISON2; break;
            case 4 : *((unsigned int*      ) var)      = POISON4; break;
            case 8 : *((unsigned long long*) var)      = POISON8; break;

            default:
                memset (var, POISON1, sizeof (varType));
            break;
        }
    }

    template<typename varType>
    bool isPoison (varType var) {

        if (var == NULL) return 1;

        switch(sizeof (varType)) {
            case 1:
                if ( *( (unsigned char*     ) &var) == POISON1) return 1;
                else                                           return 0;
                break;
            case 2:
                if ( *( (unsigned short*    ) &var) == POISON2) return 1;
                else                                           return 0;
                break;
            case 4:
                if ( *( (unsigned int*      ) &var) == POISON4) return 1;
                else                                           return 0;
                break;
            case 8:
                if ( *( (unsigned long long*) &var) == POISON8) return 1;
                else                                           return 0;
                break;
            default:
                for (int i = 0; i < sizeof (varType); i++) {

                    if (*( ( (unsigned char*) &var) + i) != POISON1) return 0;
                }
                return 1;
                break;
        }
    }

    void dumpInside (const char* varName, const char* fileName, const char* funcName, size_t line) {

        verifyHash ();

        flogprintf ( "In file %s, function %s, line %llu, Tree named %s was dumped : <br>", fileName, funcName, line, varName);

        flogprintf ( "\t" "Errors : <br>");

        errCheck ();

        logPrintErrors ();

                                        flogprintf ( "\t" "hash = %u (", hash);
        if      ( isPoison (hash)     ) flogprintf ( "POISONED)<br>")
        else                            flogprintf ( "ok)<br>")

                                        flogprintf ( "\t" "canL = 0x%X (", canL);
        if      ( isPoison (canL)     ) flogprintf ( "POISONED)<br>")
        else if ( canL      == CANL   ) flogprintf ( "ok)<br>")
        else                            flogprintf ( "NOT_OK)<br>")

                                        flogprintf ( "\t" "canR = 0x%X (", canR);
        if      ( isPoison (canR)     ) flogprintf ( "POISONED)<br>")
        else if ( canR      == CANR   ) flogprintf ( "ok)<br>")
        else                            flogprintf ( "NOT_OK)<br>")

                                        flogprintf ( "\t" "dataCanL = 0x%X (", *dataCanL);
        if      (isPoison (*dataCanL))  flogprintf ( "POISONED)<br>")
        else if (*dataCanL == CANL   )  flogprintf ( "ok)<br>")
        else                            flogprintf ( "NOT_OK)<br>")

                                        flogprintf ( "\t" "dataCanR = 0x%X (", *dataCanR);
        if      (isPoison (*dataCanR))  flogprintf ( "POISONED)<br>")
        else if (*dataCanR == CANR   )  flogprintf ( "ok)<br>")
        else                            flogprintf ( "NOT_OK)<br>")

        if (!isPoison (data) and data != NULL) {

            TreeGraphDump ();
        }

        countHash ();
    }

    void logPrintErrors () {

        errCheck ();

        char names [11][40] = {};
        int iter = 0;
        if (errCode & POISON_ACCESS        ) strcpy (names[iter++], "\t\t[POISON_ACCESS       ]<br>");
        if (errCode & BAD_CAN_L            ) strcpy (names[iter++], "\t\t[BAD_CAN_L           ]<br>");
        if (errCode & BAD_CAN_R            ) strcpy (names[iter++], "\t\t[BAD_CAN_R           ]<br>");
        if (errCode & BAD_data_CAN_L       ) strcpy (names[iter++], "\t\t[BAD_data_CAN_L      ]<br>");
        if (errCode & BAD_data_CAN_R       ) strcpy (names[iter++], "\t\t[BAD_data_CAN_R      ]<br>");
        if (errCode & NULL_data_PTR        ) strcpy (names[iter++], "\t\t[NULL_data_PTR       ]<br>");
        if (errCode & NULL_data_CAN_L_PTR  ) strcpy (names[iter++], "\t\t[NULL_data_CAN_L_PTR ]<br>");
        if (errCode & NULL_data_CAN_R_PTR  ) strcpy (names[iter++], "\t\t[NULL_data_CAN_R_PTR ]<br>");
        if (errCode & POISONED_ERRCOD      ) strcpy (names[iter++], "\t\t[POISONED_ERRCOD     ]<br>");
        if (errCode & WRONG_HASH           ) strcpy (names[iter++], "\t\t[WRONG_HASH          ]<br>");

        if (iter == 0) flogprintf ( "\t\t[ok]<br>")
        else
            for (int i = 0; i < iter; i++) flogprintf ( "%s", names[i])

    }

    size_t errCheck () {

        //checking for poison
        if (isPoison ( errCode  )   ) {

            errCode = POISONED_ERRCOD;
            return errCode;
        }

        verifyHash ();

        if (isPoison ( canL     ) or
            isPoison ( canR     ) or
            isPoison ( data     ) or
            isPoison ( dataCanL ) or
            isPoison (*dataCanL ) or
            isPoison ( dataCanR ) or
            isPoison (*dataCanR ) or
            isPoison ( hash     )   ) errCode |= POISON_ACCESS;

        //end of check

        if ( canL     != CANL      ) errCode |= BAD_CAN_L;
        if ( canR     != CANR      ) errCode |= BAD_CAN_R;
        if ( dataCanL == NULL      ) errCode |= NULL_data_CAN_L_PTR;
        else if (*dataCanL != CANL ) errCode |= BAD_data_CAN_L;
        if ( dataCanR == NULL      ) errCode |= NULL_data_CAN_R_PTR;
        else if (*dataCanR != CANR ) errCode |= BAD_data_CAN_R;
        if ( data     == NULL      ) errCode |= NULL_data_PTR;

        countHash ();

        return errCode;
    }

    void DTOR (){

        setPoison (&canL      );
        setPoison (&canR      );
        setPoison (dataCanL   );
        setPoison (dataCanR   );
        setPoison (&dataCanR  );
        setPoison (&errCode   );
        setPoison (&hash      );

        NodDtorRec ();

        free      (dataCanL );
        setPoison (&data     );
    }

    //Nod part PART

    ELEM_T* NodCtor (ELEM_T* prev = NULL, NodType type = BLANK, double val = 0, ELEM_T* left = NULL, ELEM_T* right = NULL, ELEM_T* current = NULL) {

        verifyHash ();
        ELEM_T* retVal = (current == NULL ? (ELEM_T*) calloc (1, sizeof (ELEM_T)) : current);
        assert (retVal != NULL);

        retVal->prev    = prev;
        retVal->type    = type;
        retVal->val     = val;
        retVal->left    = left;
        retVal->right   = right;
        countHash ();
        return retVal;
    }

    void NodDtorRec (ELEM_T* iter = NULL) {

        if (iter == NULL) return;

        verifyHash ();
        setPoison      (&iter->prev   );
        setPoison      (&iter->type   );
        setPoison      (&iter->val    );
        NodDtorRec     ( iter->left   );
        NodDtorRec     ( iter->right  );
        setPoison      (&iter->left   );
        setPoison      (&iter->right  );
        free           ( iter         );
        countHash ();
    }

    void DataCountHash (unsigned int* multiplier, ELEM_T* iter = NULL) {

        if (iter == NULL) return;
        countHashSeg ((char*) iter->prev, (char*) iter->right, multiplier);
        DataCountHash (multiplier, iter->left);
        DataCountHash (multiplier, iter->right);
    }

    ELEM_T* NodAddRight (ELEM_T* iter = NULL, NodType type = BLANK, double val = 0, ELEM_T* left = NULL, ELEM_T* right = NULL) {

        verifyHash ();
        iter->right = NodCtor (iter, type, val, left, right);
        assert (iter->right != NULL);
        countHash ();
        return iter->right;
    }

    ELEM_T* NodAddLeft (ELEM_T* iter = NULL, NodType type = BLANK, double val = 0, ELEM_T* left = NULL, ELEM_T* right = NULL) {

        verifyHash ();
        iter->left = NodCtor (iter, type, val, left, right);
        assert (iter->left != NULL);
        countHash ();
        return iter->left;
    }

    void PrintNod (ELEM_T* nod, int* NodNumber, int depth, FILE* picSource, int ranks[][MAX_RANKS + 1]) {

        #define picprintf(...) fprintf (picSource, __VA_ARGS__)

        nod->NodNum = *NodNumber;
        ranks[depth][0]++;
        ranks[depth][ranks[depth][0]] = *NodNumber;

        picprintf ("\t" "\"Nod_%d\" [shape = \"Mrecord\", style = \"filled\", fillcolor = \"#9feb83\", label = \"{ <prev> Prev = %p | Current = %p | type = %d | Value = %f |{ <left> Left = %p | <right> Right = %p} }\"]\n",
                    *NodNumber, nod->prev, nod, nod->type, nod->val, nod->left, nod->right);

        *NodNumber += 1;
        if (nod->left != NULL) {

            PrintNod(nod->left, NodNumber, depth + 1, picSource, ranks);
        }
        if (nod->right != NULL) {

            PrintNod (nod->right, NodNumber, depth + 1, picSource, ranks);
        }

        #undef picprintf
    }

    void PrintConnections (ELEM_T* nod, FILE* picSource) {

        #define picprintf(...) fprintf (picSource, __VA_ARGS__)

        if (nod->left != NULL) {

            picprintf ("\t" "\"Nod_%d\":left -> \"Nod_%d\";\n", nod->NodNum, nod->left->NodNum);
            PrintConnections (nod->left, picSource);
        }
        if (nod->right != NULL) {

            picprintf ("\t" "\"Nod_%d\":right -> \"Nod_%d\";\n", nod->NodNum, nod->right->NodNum);
            PrintConnections (nod->right, picSource);
        }

        #undef picprintf
    }

    void TreeGraphDump () {

        static int GraphDumpCounter = 0;

        #define picprintf(...) fprintf (picSource, __VA_ARGS__)

        char srcName[] = "GraphDumpSrc.dot";
        char picName[30] = "GraphDumpPic";
        sprintf (picName, "%d.png", GraphDumpCounter);

        FILE* picSource = fopen (srcName, "w");
        assert (picSource != NULL);

        picprintf ("digraph List_%d {" "\n", GraphDumpCounter);
        picprintf ("\t" "graph [dpi = 300];" "\n");
        picprintf ("\t" "rankdir = TB" "\n");

        int ranks[MAX_RANKS][MAX_RANKS + 1] = {0};
        int NodNum = 0;
        PrintNod (data, &NodNum, 0, picSource, ranks);

        for (int i = 0; i < MAX_RANKS and ranks[i][0] != 0; i++) {

            picprintf ("\t" "{ rank = same; ");

            for (int j = 1; j <= ranks[i][0];j++) {

                picprintf (" Nod_%d; ", ranks[i][j]);
            }

            picprintf ("}\n");
        }

        PrintConnections (data, picSource);

        picprintf ( "}");

        fclose (picSource);

        char command[200] = "";
        sprintf (command, "D:\\Graphviz\\bin\\dot.exe -Tpng %s -o %s", srcName, picName);

        system (command);

        flogprintf("<pre>\n");
        flogprintf("<h2>Tree dump</h2>\n");
        flogprintf("<img src = \"%s\" style = \"width: 55%%; height: auto\"/>\n", picName);
        flogprintf("<hr>\n");

        GraphDumpCounter++;

        #undef picprintf
    }

};