#include "predictor.h"

/////////////// STORAGE BUDGET JUSTIFICATION ////////////////

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

// Constructeur du prédicteur
PREDICTOR::PREDICTOR(char *prog, int argc, char *argv[])
{
   // La trace est tjs présente, et les arguments sont ceux que l'on désire
   if (argc != 2) {
      fprintf(stderr, "usage: %s <trace> pcbits countbits\n", prog);
      exit(-1);
   }

   uint32_t histobits    = strtoul(argv[0], NULL, 0);
   uint32_t countbits = strtoul(argv[1], NULL, 0);

   nentries = (1 << histobits);        // nombre d'entrées dans la table
   countmax = (1 << countbits) - 1; // valeur max atteinte par le compteur à saturation
   table    = new uint32_t[nentries]();
   histo_masque = (nentries-1);   //Masque de l'historique
   histo = 0;
}

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

bool PREDICTOR::GetPrediction(UINT64 PC)
{
   uint32_t v = table[histo];
   return (v > (countmax / 2)) ? TAKEN : NOT_TAKEN;
}

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

void PREDICTOR::UpdatePredictor(UINT64 PC, OpType opType, bool resolveDir, bool predDir, UINT64 branchTarget)
{
   uint32_t v = table[histo];
   table[histo] = (resolveDir == TAKEN) ? SatIncrement(v, countmax) : SatDecrement(v);

   histo = ((histo << 1) & histo_masque) | resolveDir;
}

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

void PREDICTOR::TrackOtherInst(UINT64 PC, OpType opType, bool branchDir, UINT64 branchTarget)
{
   // This function is called for instructions which are not
   // conditional branches, just in case someone decides to design
   // a predictor that uses information from such instructions.
   // We expect most contestants to leave this function untouched.
}

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////


/***********************************************************/
