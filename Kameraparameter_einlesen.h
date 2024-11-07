#ifndef Kameraparameter_einlesen_H
#define Kameraparameter_einlesen_H

// ------------------------------------------------------------------------------------------------
// Programm "Entzerren"
//   Prototypen des Unterprogramms zum Einlesen von Kameraparametern
//
// B. Lang, HS Osnabrueck
// Version Dezember 2016 (RH)
// ------------------------------------------------------------------------------------------------

#define DP  4           // Genauigkeit fuer double-Ausgaben
#define DW (DP + 6)     // Feldbreite fuer double-Ausgaben

using std::string;

// ------------------------------------------------------------------------------------------------
// Kameraparameter aus einer Datei einlesen
// ------------------------------------------------------------------------------------------------
// Parameter:
// [in]  ParameterFile : Name der Datei mit Kameraparametern
// [out] intrinsic_d   : Intrinsische Parameter der verzerrten Kamera
// [out] distCoeffs    : Verzerrungsparameter
// [out] rotVect       : Rodriguezvektor
// [out] transVect     : Translationsvektor
// Return:
//  0 bei fehlerfreiem Einlesen
// -1 bei einem Fehler
// ------------------------------------------------------------------------------------------------
int Kameraparameter_einlesen(
      const string& ParameterFile,  // parameter file name
      double intrinsic_d[3][3],     // intrinsics
      double distCoeffs[5],         // distortion
      double rotVect[3],            // rotation
      double transVect[3]           // translation
);

#endif
