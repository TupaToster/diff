#pragma once
#pragma GCC diagnostic ignored "-Wmultichar"
#include <stdio.h>
#include <stdlib.h>
#include "flog.h"

#ifndef NDEBUG
#define dump(clas) (clas).dumpInside (#clas, __FILE__, __FUNCTION__, __LINE__) ///< NEW_STRUCT dump macros
#else
#define dump(clas) ;
#endif

#define pasta(a) #a

enum NodType {

    BLANK = 0,
    #define DEFCMD(name, priority, sign, calc, diff, get) name = priority,

    #include "codegen.h"


    #undef DEFCMD
    CONSTANT = (int) (1<<30),
    X = (int) ((1<<30) + 1)
};

struct Nod {

    Nod*    prev    = NULL;
    NodType type    = BLANK;
    double  val     = 0;
    int     NodNum  = 0;
    int     diff    = 0;
    Nod*    left    = NULL;
    Nod*    right   = NULL;
};

template<typename ELEM_T>
class Tree {

    // constants
    static constexpr unsigned int       CANL      = 0xDEADBEEF; ///< Left cannary of a structure
    static constexpr unsigned int       CANR      = 0xD34DB33F; ///< Right cannary of a structure
    static constexpr unsigned char      POISON1   = 0xBD; ///< 1 byte Poison
    static constexpr unsigned short     POISON2   = 0xBDCF; ///< 2 byte Poison
    static constexpr unsigned int       POISON4   = 0xBADC0FEE; ///< 4 byte Poison
    static constexpr unsigned long long POISON8   = 0xBADC0FEEF04DED32; ///< 8 byte Poison
    static constexpr unsigned int       HASH_MULT = 37u; ///< Multiplier for hash
    static constexpr unsigned int       MAX_RANKS = 100;

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

    unsigned int   canL      = CANL; ///< left cannary of struct
    unsigned int   hash      = 0;    ///< hash value
    size_t         errCode   = ok;   ///< error code
    ELEM_T*        data      = NULL; ///< Ptr to data
    unsigned int*  dataCanL  = NULL; ///< left cannary of data
    unsigned int*  dataCanR  = NULL; ///< right cannary of data
    unsigned int   canR      = CANR; ///< right cannary of struct

    void countHashSeg (void* left, void* right, unsigned int* multiplier) {

        assert (left != NULL);
        assert (right != NULL);
        assert (left < right);
        assert (multiplier != NULL);

        for (; left < right; left++) {

            hash += ((unsigned int) * (char*) left) * (*multiplier);
            *multiplier *= HASH_MULT;
        }
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

    unsigned int countHash () {

        unsigned int multiplier = 1;
        hash = 0;

        countHashSeg (&canL, &canR, &multiplier);

        if (dataCanL != NULL) countHashSeg (dataCanL, dataCanR, &multiplier);

        return hash;
    }

    bool verifyHash () {

        if (hash != countHash()) {

            errCode |= WRONG_HASH;
            return false;
        }
        return true;
    }

    size_t errCheck () {

        //checking for poison
        if (isPoison (&errCode  )   ) {

            errCode = POISONED_ERRCOD;
            return errCode;
        }

        verifyHash ();

        if (isPoison (&canL     ) or
            isPoison (&canR     ) or
            isPoison (&data     ) or
            isPoison (&dataCanL ) or
            isPoison ( dataCanL ) or
            isPoison (&dataCanR ) or
            isPoison ( dataCanR ) or
            isPoison (&hash     )   ) errCode |= POISON_ACCESS;

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

    // Nod part

    const char* NodTypeStr (NodType type) {

        switch (type) {

            case BLANK: return "BLANK";

            case CONSTANT: return "CONSTANT";

            case X: return "X";

            #define DEFCMD(name, priority, sign, calc, diff, get) case name : return #name ;

            #include "..\lib\codegen.h"

            #undef DEFCMD

            default:
                printf ("%d\n", type);
                return "UNKNOWN_TYPE";
        }
    }

    void TreeGraphDump () {

        static int GraphDumpCounter = 0;

        if (GraphDumpCounter == 0) {

            system ("rm *.png");
        }

        #define picprintf(...) fprintf (picSource, __VA_ARGS__)

        char srcName[] = "GraphDumpSrc.dot";
        char picName[30] = "GraphDumpPic";
        sprintf (picName, "%d.png", GraphDumpCounter);

        FILE* picSource = fopen (srcName, "w");
        assert (picSource != NULL);

        picprintf ("digraph List_%d {" "\n", GraphDumpCounter);
        picprintf ("\t" "graph [dpi = 100];" "\n");
        picprintf ("\t" "rankdir = TB" "\n");

        int ranks[MAX_RANKS][MAX_RANKS + 1] = {0};
        int NodNum = 0;

        verifyHash ();

        PrintNod (data, &NodNum, 0, picSource, ranks);

        countHash ();

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

        flogprintf("<h2>Tree dump</h2>\n");
        flogprintf("<img src = \"%s\" style = \"width: 55%%; height: auto\"/>\n", picName);

        GraphDumpCounter++;

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
        // uncomment for debug
        // if (nod->prev != NULL) {

        //     picprintf ("\t" "\"Nod_%d\":prev -> \"Nod_%d\";\n", nod->NodNum, nod->prev->NodNum);
        // }

        #undef picprintf
    }

    void PrintNod (ELEM_T* nod, int* NodNumber, int depth, FILE* picSource, int ranks[][MAX_RANKS + 1]) {

        #define picprintf(...) fprintf (picSource, __VA_ARGS__)

        verifyHash ();
        nod->NodNum = *NodNumber;
        countHash ();

        ranks[depth][0]++;
        ranks[depth][ranks[depth][0]] = *NodNumber;

        picprintf ("\t" "\"Nod_%d\" [shape = \"Mrecord\", style = \"filled\", fillcolor = \"#9feb83\", label = \"{ <prev> Prev = %p | Current = %p | type = %s | Value = %f |{ <left> Left = %p | <right> Right = %p} }\"]\n",
                    *NodNumber, nod->prev, nod, NodTypeStr (nod->type), (double) nod->val, nod->left, nod->right);

        *NodNumber += 1;
        if (nod->left != NULL) {

            PrintNod(nod->left, NodNumber, depth + 1, picSource, ranks);
        }
        if (nod->right != NULL) {

            PrintNod (nod->right, NodNumber, depth + 1, picSource, ranks);
        }

        #undef picprintf
    }

    public:

    Tree (unsigned int _size = 1) :
        canL (CANL), canR (CANR), hash(0), errCode (ok) {

        dataCanL = (unsigned int*) calloc (sizeof (ELEM_T) * _size + 2 * sizeof (unsigned int), 1);
        assert (dataCanL != NULL);
        data = (ELEM_T*) (dataCanL + 1);
        assert (data != NULL);
        dataCanR = (unsigned int*) (data + _size);
        assert (dataCanR != NULL);

       *dataCanL = CANL;
       *dataCanR = CANR;

        countHash();
    }

    ELEM_T* getdata () {

        return data;
    }

    unsigned int geterrCode () {

        return errCode;
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
    bool isPoison (varType* var) {

        if (var == NULL) return 1;

        switch(sizeof (varType)) {

            case 1:
                if ( *( (unsigned char*     ) &var) == POISON1) return 1;
                else                                            return 0;
            break;

            case 2:
                if ( *( (unsigned short*    ) &var) == POISON2) return 1;
                else                                            return 0;
            break;

            case 4:
                if ( *( (unsigned int*      ) &var) == POISON4) return 1;
                else                                            return 0;
            break;

            case 8:
                if ( *( (unsigned long long*) &var) == POISON8) return 1;
                else                                            return 0;
            break;

            default:
                for (int i = 0; i < sizeof (varType); i++) {

                    if (*( ( (unsigned char*) &var) + i) != POISON1) return 0;
                }
            return 1;
            break;
        }
    }

    void DTOR () {

        setPoison (&canL);
        setPoison (&canR);
        setPoison (&errCode);
        setPoison (&hash);

        if (dataCanL != NULL) {

            setPoison (dataCanL);
            setPoison (dataCanR);
            dataCanL++;
            for (; (void*) data < (void*) dataCanR; data++) setPoison (data);
            free (dataCanL);
            setPoison (&dataCanL);
            setPoison (&data);
            setPoison (&dataCanR);
        }
    }

    void dumpInside (const char* name, const char* fileName, const char* funcName, size_t line) {

        errCheck ();

        flogprintf ("<pre>" "In file %s, function %s, line %llu, Tree named %s was dumped : <br>",
            fileName, funcName, line, name);

        flogprintf ("\t" "Errors : <br>");

        logPrintErrors ();

                                         flogprintf ( "\t" "hash     = %u (", hash);
        if      ( isPoison (&hash)     ) flogprintf ( "POISONED)<br>")
        else                             flogprintf ( "ok)<br>")

                                         flogprintf ( "\t" "canL     = 0x%X (", canL);
        if      ( isPoison (&canL)     ) flogprintf ( "POISONED)<br>")
        else if ( canL      == CANL    ) flogprintf ( "ok)<br>")
        else                            flogprintf ( "NOT_OK)<br>")

                                         flogprintf ( "\t" "canR     = 0x%X (", canR);
        if      ( isPoison (&canR)     ) flogprintf ( "POISONED)<br>")
        else if ( canR      == CANR    ) flogprintf ( "ok)<br>")
        else                             flogprintf ( "NOT_OK)<br>")

                                         flogprintf ( "\t" "dataCanL = 0x%X (", *dataCanL);
        if      (isPoison (dataCanL)   ) flogprintf ( "POISONED)<br>")
        else if (*dataCanL == CANL     ) flogprintf ( "ok)<br>")
        else                             flogprintf ( "NOT_OK)<br>")

                                         flogprintf ( "\t" "dataCanR = 0x%X (", *dataCanR);
        if      (isPoison (dataCanR)   ) flogprintf ( "POISONED)<br>")
        else if (*dataCanR == CANR     ) flogprintf ( "ok)<br>")
        else                             flogprintf ( "NOT_OK)<br>")

        if (!isPoison (data) and data != NULL) {

            TreeGraphDump ();
        }

        flogprintf ("</pre><hr>\n");
        countHash ();
    }

    //Nod part

    void setNod (ELEM_T* current, NodType type = BLANK, double val = 0, int NodNum = 0, int diff = 0) {

        verifyHash ();
        current->type = type;
        current->val = val;
        current->NodNum = NodNum;
        current->diff = diff;
        countHash ();
    }

    void setDiff (ELEM_T* current, int diff = 0) {

        verifyHash ();
        current->diff = diff;
        countHash ();
    }

    ELEM_T* NodCtor (ELEM_T* prev = NULL, NodType type = BLANK, double val = 0, ELEM_T* left = NULL, ELEM_T* right = NULL, ELEM_T* current = NULL, int NodNum = 0, int diff = 0) {

        errCheck ();
        ELEM_T* retVal = (current == NULL ? (ELEM_T*) calloc (1, sizeof (ELEM_T)) : current);
        assert (retVal != NULL);

        retVal->prev    = prev;
        retVal->type    = type;
        retVal->val     = val;
        retVal->left    = left;
        retVal->right   = right;
        retVal->diff    = diff;
        retVal->NodNum  = NodNum;
        countHash ();
        return retVal;
    }

    void NodDtorRec (ELEM_T* iter = NULL) {

        if (iter == NULL) return;

        errCheck ();
        setPoison (&iter->type);
        setPoison (&iter->val);
        setPoison (&iter->NodNum);
        setPoison (&iter->diff);

        NodDtorRec (iter->left);
        NodDtorRec (iter->right);

        setPoison (&iter->left);
        setPoison (&iter->right);

        if (iter->prev != NULL)
            if (iter->prev->left == iter) iter->prev->left = NULL;
            else iter->prev->right = NULL;

        setPoison (&iter->prev);
        free (iter);
        countHash ();
    }

    ELEM_T* NodAddRight (ELEM_T* iter, NodType type = BLANK, double val = 0, ELEM_T* left = NULL, ELEM_T* right = NULL) {

        if (iter->prev != NULL) {
            if (iter->prev->left != iter and iter->prev->right != iter) dumpInside ("blyat", __FILE__, __FUNCTION__, __LINE__);
            assert (iter->prev->left == iter or iter->prev->right == iter);
        }
        assert (iter != NULL);
        errCheck ();

        iter->right = NodCtor (iter, type, val, left, right);
        assert (iter->right != NULL);

        countHash ();
        return iter->right;
    }

    ELEM_T* NodAddLeft (ELEM_T* iter, NodType type = BLANK, double val = 0, ELEM_T* left = NULL, ELEM_T* right = NULL) {

        if (iter->prev != NULL) {
            if (iter->prev->left != iter and iter->prev->right != iter) dumpInside ("blyat", __FILE__, __FUNCTION__, __LINE__);
            assert (iter->prev->left == iter or iter->prev->right == iter);
        }
        assert (iter != NULL);
        errCheck ();

        iter->left = NodCtor (iter, type, val, left, right);
        assert (iter->left != NULL);

        countHash ();
        return iter->right;
    }

    void NodMoveLeft (ELEM_T* iter, NodType type = BLANK, double val = 0) {

        if (iter->prev != NULL) {
            if (iter->prev->left != iter and iter->prev->right != iter) dumpInside ("blyat", __FILE__, __FUNCTION__, __LINE__);
            assert (iter->prev->left == iter or iter->prev->right == iter);
        }
        errCheck ();
        NodAddLeft (iter, iter->type, iter->val, iter->left, iter->right);
        NodCtor (iter->prev, type, val, iter->left, NULL, iter);
        if (iter->left->left != NULL) iter->left->left->prev = iter->left;
        if (iter->left->right != NULL) iter->left->right->prev = iter->left;
        countHash ();
    }

    void NodMoveRight (ELEM_T* iter, NodType type = BLANK, double val = 0) {

        if (iter->prev != NULL) {
            if (iter->prev->left != iter and iter->prev->right != iter) dumpInside ("blyat", __FILE__, __FUNCTION__, __LINE__);
            assert (iter->prev->left == iter or iter->prev->right == iter);
        }
        errCheck ();
        NodAddRight (iter, iter->type, iter->val, iter->left, iter->right);
        NodCtor (iter->prev, type, val, NULL, iter->right, iter);
        if (iter->right->left != NULL) iter->right->left->prev = iter->right;
        if (iter->right->right != NULL) iter->right->right->prev = iter->right;
        countHash ();
    }

    void NodUnMoveRight (ELEM_T* iter) {

        if (iter->prev != NULL) {
            if (iter->prev->left != iter and iter->prev->right != iter) dumpInside ("blyat", __FILE__, __FUNCTION__, __LINE__);
            assert (iter->prev->left == iter or iter->prev->right == iter);
        }
        errCheck ();
        ELEM_T* right = iter->right;
        NodCtor (iter->prev, iter->right->type, iter->right->val, iter->right->left, iter->right->right, iter);
        NodCtor (NULL, BLANK, 0, NULL, NULL, right);
        NodDtorRec (right);
        if (iter->right != NULL) iter->right->prev = iter;
        if (iter->left != NULL) iter->left->prev = iter;
        countHash ();
    }

    void NodUnMoveLeft (ELEM_T* iter) {

        if (iter->prev != NULL) {
            if (iter->prev->left != iter and iter->prev->right != iter) dumpInside ("blyat", __FILE__, __FUNCTION__, __LINE__);
            assert (iter->prev->left == iter or iter->prev->right == iter);
        }
        errCheck ();
        ELEM_T* left = iter->left;
        NodCtor (iter->prev, iter->left->type, iter->left->val, iter->left->left, iter->left->right, iter);
        NodCtor (NULL, BLANK, 0, NULL, NULL, left);
        NodDtorRec (left);
        if (iter->right != NULL) iter->right->prev = iter;
        if (iter->left != NULL) iter->left->prev = iter;
        countHash ();
    }

    void NodRecCpy (ELEM_T* src, ELEM_T* dst) {

        assert (dst != NULL);
        assert (src != NULL);

        errCheck ();
        dst = NodCtor (dst->prev, src->type, src->val, NULL, NULL, dst);
        if (src->left != NULL) {

            NodAddLeft (dst);
            NodRecCpy (src->left, dst->left);
        }
        if (src->right != NULL) {

            NodAddRight (dst);
            NodRecCpy (src->right, dst->right);
        }
        countHash ();
    }

    int getTreeSize (ELEM_T* iter) {

        assert (iter != NULL);
        return (iter->left == NULL ? 0 : getTreeSize (iter->left)) + (iter->right == NULL ? 0 : getTreeSize (iter->right)) + 1;
    }
};