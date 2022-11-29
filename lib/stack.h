#pragma once
#include <stdio.h>
#include <stdlib.h>
#include "flog.h"

#ifndef NDEBUG
#define dump(clas) clas.dumpInside (#clas, __FILE__, __FUNCTION__, __LINE__) ///< NEW_STRUCT dump macros
#else
#define dump(clas) ;
#endif


template<typename ELEM_T>
class Stack {

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
    unsigned int   size      = 0;
    unsigned int   capacity  = 0;
    ELEM_T*        data      = NULL; ///< Ptr to data
    unsigned int*  dataCanL  = NULL; ///< left cannary of data
    unsigned int*  dataCanR  = NULL; ///< right cannary of data
    unsigned int   canR      = CANR; ///< right cannary of struct

    public:

    ELEM_T* getdata () {

        return data;
    }

    unsigned int geterrCode () {
        return errCode;
    }

    Stack () {

        canL      = CANL;
        canR      = CANR;
        hash      = 0;
        errCode   = ok;
        size      = 0;
        capacity  = 4;

        data     = (ELEM_T*) calloc (capacity * sizeof (ELEM_T) + 2 * sizeof (unsigned int), 1);
        assert (data != NULL);
        dataCanL = (unsigned int*) data;
        data     = (ELEM_T*)(((unsigned int*) data) + 1);
        dataCanR = (unsigned int*) (data + capacity);

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

        flogprintf ( "In file %s, function %s, line %llu, Stack named %s was dumped : <br>", fileName, funcName, line, varName);

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

                                        flogprintf ("\t" "size = %u (", size);
        if      ( isPoison (size)     ) flogprintf ("POISONED)<br>")
        else                            flogprintf ("ok)<br>")

                                        flogprintf ("\t" "capacity = %u (", capacity);
        if      ( isPoison (capacity) ) flogprintf ("POISONED)<br>")
        else                            flogprintf ("ok)<br>")

                                        flogprintf ( "\t" "dataCanL = 0x%X (", *dataCanL);
        if      (isPoison (*dataCanL))  flogprintf ( "POISONED)<br>")
        else if (*dataCanL == CANL   )  flogprintf ( "ok)<br>")
        else                            flogprintf ( "NOT_OK)<br>")

                                        flogprintf ( "\t" "dataCanR = 0x%X (", *dataCanR);
        if      (isPoison (*dataCanR))  flogprintf ( "POISONED)<br>")
        else if (*dataCanR == CANR   )  flogprintf ( "ok)<br>")
        else                            flogprintf ( "NOT_OK)<br>")

        if (!isPoison (data) and data != NULL and size != 0) {

            flogprintf ("\t" "%s.stack : <br>", varName);
            for (int i = 0; i < capacity; i++) {

                flogprintf ("\t\t" "%s.data[%d] : " , varName, i);   //%u -> Stack Elem_t Format
                flogIntern (&data[i], typeid (ELEM_T).name (), NULL, sizeof (ELEM_T), NULL, NULL, 0);
                if (isPoison (data[i])) flogprintf ("^^(POISONED)^^<br>");
            }
        }

        flogprintf ("\t" "End of stack" "<br>" "End of dump <br>");
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
        verifyHash ();

        if (isPoison ( errCode  )   ) {

            errCode = POISONED_ERRCOD;
            countHash ();
            return errCode;
        }


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
        if ( size > capacity       ) errCode |= WRONG_SIZE;

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
        setPoison (&size      );

        for (int i = 0; i < capacity;i++) setPoison (data[i]);
        setPoison (&capacity  );

        free      (dataCanL   );
        setPoison (&data      );
    }

    //Stack only part

    void push (ELEM_T val) {

        errCheck ();

        if (size == capacity) reallocSizeShift (1);

        data[size] = val;
        size++;

        errCheck ();
    }

    ELEM_T pop () {

        errCheck ();

        if (size == 0) return 0;
        ELEM_T retVal = data[size - 1];
        data[size-1] = 0;
        size--;

        countHash ();

        if (capacity > 4 and size == capacity * 3 / 8) reallocSizeShift (-1);

        errCheck ();

        return retVal;
    }

    void reallocSizeShift (int delta) {

        errCheck ();

        *((unsigned int*) (data + capacity)) = 0;

        if (delta > 0) {

            data = (ELEM_T*) calloc (capacity * sizeof (ELEM_T) * 2 + 2 * sizeof (unsigned int), 1);
            memcpy (data, dataCanL, capacity * sizeof (ELEM_T) + sizeof (unsigned int));
            capacity *= 2;

        }
        else if (delta < 0) {

            data = (ELEM_T*) calloc (capacity * sizeof (ELEM_T) / 2  + 2 * sizeof (unsigned int), 1);
            memcpy (data, dataCanL, capacity * sizeof (ELEM_T) / 2 + sizeof (unsigned int));
            capacity /= 2;
        }

        free (dataCanL);

        dataCanL = (unsigned int*) data;

        data = (ELEM_T*) (((unsigned int*) data) + 1);

        dataCanR = (unsigned int*) (data + capacity);
        *dataCanR = CANR;

        errCheck ();
    }
};
