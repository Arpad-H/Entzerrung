// ------------------------------------------------------------------------------------------------
// Programm "Entzerren"
//   Unterprogramm zum Einlesen der Kameraparameter
//
// B. Lang, HS Osnabrueck
// Version Dezember 2016 (RH)
// ------------------------------------------------------------------------------------------------

#include <iostream>
#include <iomanip>
#include <fstream>
//#include <cstdlib>
#include "Kameraparameter_einlesen.h"

using std::ifstream;
using std::cout;
using std::cerr;
using std::endl;
using std::setw;

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
	const string& ParameterFile,
	double intrinsic_d[3][3],
	double distCoeffs[5],
	double rotVect[3],
	double transVect[3]
)
{
	double fx, fy;
	double cx, cy;
	double k1, k2, k3;
	double p1, p2;
	double rx, ry, rz;
	double tx, ty, tz;
	double dummy;
	string separator, line;
	ifstream infile(ParameterFile.c_str());

	if(!infile.is_open()) {
		return -1;
	}

	getline(infile, line);
	infile >> fx    >> separator >> dummy >> separator >> cx;
	getline(infile, line);
	infile >> dummy >> separator >> fy    >> separator >> cy;
	getline(infile, line);
	getline(infile, line);
	getline(infile, line);
	infile >> k1 >> separator >> k2 >> separator >> k3;
	getline(infile, line);
	getline(infile, line);
	infile >> p1 >> separator >> p2;
	getline(infile, line);
	getline(infile, line);
	infile >> rx >> separator >> ry >> separator >> rz;
	getline(infile, line);
	infile >> tx >> separator >> ty >> separator >> tz;
	getline(infile, line);

	cout << "Eingelesene Kameraparameter: " << endl;
	cout << "fx,fy,cx,cy:" << setw(DW) << fx << "," << setw(DW) << fy << ","
             << setw(DW) << cx << "," << setw(DW) << cy << endl;
	cout << "k1,k2,k3:   " << setw(DW) << k1 << "," << setw(DW) << k2 << "," << setw(DW) << k3 << endl;
	cout << "p1,p2:      " << setw(DW) << p1 << "," << setw(DW) << p2 << endl;
	cout << "rx,ry,rz:   " << setw(DW) << rx << "," << setw(DW) << ry << "," << setw(DW) << rz << endl;
	cout << "tx,ty,tz:   " << setw(DW) << tx << "," << setw(DW) << ty << "," << setw(DW) << tz << endl;
	cout << endl;

	intrinsic_d[0][0] = fx;
	intrinsic_d[0][1] = 0.0;
	intrinsic_d[0][2] = cx;
	intrinsic_d[1][0] = 0.0;
	intrinsic_d[1][1] = fy;
	intrinsic_d[1][2] = cy;
	intrinsic_d[2][0] = 0.0;
	intrinsic_d[2][1] = 0.0;
	intrinsic_d[2][2] = 1.0;
	distCoeffs[0] = k1;
	distCoeffs[1] = k2;
	distCoeffs[2] = p1;
	distCoeffs[3] = p2;
	distCoeffs[4] = k3;
	rotVect[0]    = rx;
	rotVect[1]    = ry;
	rotVect[2]    = rz;
	transVect[0]  = tx;
	transVect[1]  = ty;
	transVect[2]  = tz;

	return 0;
}
