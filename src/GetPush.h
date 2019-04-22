// GetPush.h  bloquant repris pour Libx19 corr 190303
// main PP  #include "GetPush.h" 
// utilise PushOn et LedOn comme défini dans Witty.h 
// si pin partagée, voir  Dixi

byte cntPush, cntOff = 0, cli=0; 
//<-- Fonction GetPous --> rend nbre d'actions 1 2 3..
  enum { WaitFirst,  Pushing, NotPushing } npl= WaitFirst; 
byte GetPush() { 
  cntPush=0; cntOff = 0;
  while (cntOff < (500/5)) {  // attend dernier push 0.5s
    switch (npl) {
     case WaitFirst: // on clignote en attendant un push
      if (!PushOn) {
        LedOff; DelMs (2);
        if (cli++ == 40) { cli = 0; LedOn; DelMs (5);}  
        LedOff;  break;
      }
      npl= Pushing; break;
     case Pushing:  // on attend le relâchemnent
      LedOn;
      if (PushOn) { break; } //attend relaché
      LedOff;
      cntPush++ ;  cntOff= 0 ;
      npl = NotPushing ; break ;
     case NotPushing:  // on attend un peu s'il y a une pression suivant
      DelMs (5);
      cntOff++;  //200x5 = 1 sec 
      if (!PushOn) break ; //on relâche
      npl = Pushing ;
      break;
     } // end switch
  } // end while
  npl = WaitFirst;  // prépare appel suivant
  //  clignote cntPous fois
  for (cli=0; cli<cntPush; cli++) {
    LedOn ; DelMs(100); LedOff ; DelMs(200);
  }  // end for
  cntOff = 0;
  return cntPush ;
}


