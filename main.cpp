// ------------------------------------------------------------------------------------------------
// Programm "Entzerren"
//   Das Programm dient zum Entzerren von BMP-Bildern
//   - Der Name des zu entzerrenden Bildes wird (ohne ".bmp"-Endung)
//     dem Programm als Argument uebergeben.
//     (Bei Code::Blocks werden die Argumente unter
//      "Project"->"Set programs' arguments..." angegeben)
//   - Die f�r die Entzerrung ben�tigten Kameraparameter werden mittels
//     der Datei "Kameraparameter.csv" uebergeben.
//
// B. Lang, HS Osnabrueck
// Version Dezember 2016 (RH)
// ------------------------------------------------------------------------------------------------

#include <iostream>
#include <iomanip>
#include <cstring> // strerror
#include "Img.h"
#include "BmpRead.h"
#include "BmpWrite.h"
#include "Entzerren.h"
#include "Kameraparameter_einlesen.h"

#define DP  4           // Genauigkeit fuer double-Ausgaben
#define DW (DP + 6)     // Feldbreite fuer double-Ausgaben

// ------------------------------------------------------------------------------------------------
// Intrinsische Parameter der entzerrten, senkrecht ausgerichteten Kamera bestimmen
// (Aufgabe 3)
// ------------------------------------------------------------------------------------------------
// Parameter:
// [in]  intrinsic_d : Intrinsische Parametern der realen Kamera
// [in]  width_d     : Breite des Bildes der realen Kamera
// [in]  height_d    : Hoehe des Bildes der realen Kamera
// [in]  distCoeffs  : Verzerrungsparameter
// [in]  rotVect     : Rotationsvektor
// [out] intrinsic   : Intrinische Parameter der senkrecht ausgerichteten Kamera
// ------------------------------------------------------------------------------------------------
void Calculate_Intrinsics(const double intrinsic_d[3][3],
                          unsigned int width_d, unsigned int height_d,
                          const double distCoeffs[5], const double rotVect[3],
                          double intrinsic[3][3], unsigned int& width, unsigned int& height)
{
    // 3x3-Matrix f�r die intrinsischen Parameter der neuen Kamera definieren und fuellen:
    // - fx vom verzerrten Bild als Brennweite in u- und in v-Richtung �bernehmen
    // - cx,cy zunaechst zu 0 setzen

    // TODO: Code einkommentieren und ergaenzen
     double fx = intrinsic_d[0][0];
     double cx = 0.0;
     double cy = 0.0;

    // Zunaechst vorlauefige Werte verwenden

    // TODO: Code einkommentieren und ergaenzen
     double preliminary[3][3] = {
     	{fx,             0.0 ,              cx},
     	{0.0,             fx,               cy},
     	{0.0,             0.0,               1.0}
     };

    // Vektor fuer Positionen der vier Eckpunkte des verzerrten Bilds definieren und fuellen

    // TODO: Code einkommentieren und ergaenzen
    std::vector<std::pair<double, double>> corners_d;
    corners_d.push_back({0.0, 0.0});                // left bottom
    corners_d.push_back({0.0, static_cast<double>(height_d)});  // left top
    corners_d.push_back({static_cast<double>(width_d), 0.0});   // right bottom
    corners_d.push_back({static_cast<double>(width_d), static_cast<double>(height_d)});  // right top

    // Eckpunkte des verzerrten Bildes im unverzerrten Bild (noch mit cx=0,cy=0) bestimmen

    // TODO: Code einkommentieren und ergaenzen
     vector<pair<double, double> > corners;
     UndistorePoints( corners,
                      preliminary,
                      corners_d,
                      intrinsic_d,
                      distCoeffs,
                      rotVect          );

    // Minimale um maximale Positionen der vier Eckpunkte des verzerrten Bilds
    // (umschlie�endes Rechteck) im entzerrten Bild bestimmen

    // TODO: Code zur Suche der minimalen und maximalen Werte fuer u und v erstellen
    double min_u = corners[0].first, max_u = corners[0].first;
    double min_v = corners[0].second, max_v = corners[0].second;
    for (const auto& corner : corners) {
        if (corner.first < min_u) min_u = corner.first;
        if (corner.first > max_u) max_u = corner.first;
        if (corner.second < min_v) min_v = corner.second;
        if (corner.second > max_v) max_v = corner.second;
    }


    // Aus den minimalen und maximalen Positionen der vier Eckpunkte im entzerrten Bild
    // geeignete Werte fuer cx und cy bestimmen
    // TODO: Code einkommentieren und ergaenzen

    cx = (max_u + min_u) / 2.0;
    cy = (max_v + min_v) / 2.0;

    // Dimensionen des entzerrten Bildes setzen
    // TODO: Code einkommentieren und ergaenzen
    width = static_cast<unsigned int>(max_u - min_u);
    height = static_cast<unsigned int>(max_v - min_v);


    // Intrinsische Parameter der ausgerichteten Kamera setzen

    // TODO: Code einkommentieren
    intrinsic[0][0] = fx; intrinsic[0][1] = 0.0; intrinsic[0][2] = cx;
    intrinsic[1][0] = 0.0; intrinsic[1][1] = fx; intrinsic[1][2] = cy;
    intrinsic[2][0] = 0.0; intrinsic[2][1] = 0.0; intrinsic[2][2] = 1.0;

}

// ------------------------------------------------------------------------------------------------
// Eine Rotationsmatrix ausgeben
// ------------------------------------------------------------------------------------------------
// Parameter:
// [in] R : Rotationsmatrix
// ------------------------------------------------------------------------------------------------
void Print_RotMat(double R[3][3])
{
    cout << "Rotationsmatrix (berechnet aus rx, ry, rz):" << endl;
    cout << setw(DW) << R[0][0] << setw(DW) << R[0][1] << setw(DW) << R[0][2] << endl;
    cout << setw(DW) << R[1][0] << setw(DW) << R[1][1] << setw(DW) << R[1][2] << endl;
    cout << setw(DW) << R[2][0] << setw(DW) << R[2][1] << setw(DW) << R[2][2] << endl;
    cout << endl;
}

// ------------------------------------------------------------------------------------------------
// Hauptprogramm
// ------------------------------------------------------------------------------------------------
// Parameter:
// [in] argv[1] : Name des Eingangsbildes
// ------------------------------------------------------------------------------------------------
int main(int argc, char* argv[])
{
    string filename;

    cout << "BV-Praktikum: Entzerren" << endl << endl;

    if(argc != 2)
    {
        cerr << "Aufruf: " << argv[0] << " <ImageName>" << endl;
        return -1;
    }

    cout << fixed << setprecision(DP);

    // Kameraparameter von der Kalibrierung einlesen
    string ParameterFile("Kameraparameter.csv"); // Datei mit Kameraparametern
    double intrinsic_d[3][3]; // Intrinsische Parameter der realen (verzerrten) Kamera
    double distCoeffs[5];     // Verzerrungsparameter
    double rotVect[3];        // Rotationsvektor
    double transVect[3];      // Translationsvektor
    if(Kameraparameter_einlesen(ParameterFile, intrinsic_d, distCoeffs, rotVect, transVect))
    {
        cerr << "Fehler beim Lesen von " << ParameterFile << ": " << strerror(errno) << endl;
        return -1;
    }
    cout << "Lese Kameraparameter aus Datei: " << ParameterFile << endl << endl;

    // Rotationsmatrix aus dem Rodriguez-Vektor berechnen
    double R[3][3];           // Rotationsmatrix
    RotMat_from_Rodriguez(R, rotVect);
    Print_RotMat(R);

    // Verzerrtes Bild einlesen
    string ImageName(argv[1]); // Name der zu bearbeitenden Datei
    Img<RGB_Pixel> img_d;                           // Verzerrtes Bild
    try
    {
        filename = ImageName + ".bmp";
        BmpRead(filename.c_str()) >> img_d; // Verzerrtes Bild einlesen
        cout << "Lese verzerrtes Bild: " << filename << endl << endl;
    }
    catch(const char * s)
    {
        cerr << "Fehler beim Lesen von " << filename << ": " << s << endl;
        return -1;
    }

    img_d.Margin_Constant(RGB_Pixel(0, 0, 0));      // Randbehandlung setzen

    // Parameter fuer entzerrte Bilder berechnen
    double intrinsic[3][3]; // Intrinsische Parameter der entzerrten, senkrecht ausgereichteten Kamera
    unsigned int width, height; // Groesse des entzerrten Bildes
    Calculate_Intrinsics(intrinsic_d, img_d.Width(), img_d.Height(), distCoeffs, rotVect, intrinsic, width, height);
    cout << "Parameter fuer entzerrte Bilder:" << endl;
    cout << "cx:" << setw(DW) << intrinsic[0][2] << endl
         << "cy:" << setw(DW) << intrinsic[1][2] << endl
         << "f: " << setw(DW) << intrinsic[0][0] << endl << endl;

    // Entzerrung durchf�hren: "img" aus "img_d" berechnen
    Img<RGB_Pixel> img(width, height); // entzerrtes Bild

    // TODO: Code einkommentieren
    UndistoreImage(img,intrinsic, img_d, intrinsic_d, distCoeffs, rotVect );

    // Entzerrtes Bild "img" wegspeichern
    try
    {
        filename = ImageName + "_entzerrt.bmp";
        BmpWrite(filename.c_str(), img);
        cout << "Schreibe entzerrtes Bild: " << filename << endl;
    }
    catch(const char * s)
    {
        cerr << "Fehler beim Schreiben von " << filename << ": " << s << endl;
        return -1;
    }

    return 0;
}
