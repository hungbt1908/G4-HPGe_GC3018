#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class G4Material;
class G4VisAttributes;
class G4Box;
class G4Tubs;
class G4VPhysicalVolume;


class DetectorConstruction : public G4VUserDetectorConstruction
{
public:
    DetectorConstruction();
    ~DetectorConstruction();

public:
    G4VPhysicalVolume* Construct() override;
    void ConstructSDandField() override;

    // get methods
    //
    G4LogicalVolume* GetScoringVolume() const { return fScoringVolume; }

private:
    // methods
    //
    void DefineMaterials();
    G4VPhysicalVolume* DefineVolumes();

private:  
    // data members
    // Solid volume
    G4Box* solidWorld;
    G4Tubs* solid_Endcap_Outer, * solid_Endcap_Inner, * solid_Ge, * solid_Electrode_Hole,
      * solid_Window_Electrode, * solid_Outer_Electrode_P1, * solid_Outer_Electrode_P2,
      * solid_Crystal_Holder_P1, * solid_Crystal_Holder_P2, * solid_Crystal_Holder_P3,
      * solid_Crystal_Holder_P4, * solid_Crystal_Holder_P5, * solid_Crystal_Holder_P6,
      * solid_Crystal_Holder_P7, * solid_Crystal_Holder_P8, * solid_Crystal_Holder_P9,
      * solid_Teflon_Insulator_P1, * solid_Teflon_Insulator_P2, * solid_Signal_Core;

    // Logical volume
    G4LogicalVolume* logicWorld, * logic_Endcap, * logic_Vacuum_Volume, * logic_Ge_Crystal,
      * logic_Window_Electrode, * logic_Outer_Electrode, * logic_Crystal_Holder,
      * logic_Teflon_Insulator, * logic_Signal_Core;

    // Physical volume
    G4VPhysicalVolume* physWorld;

    // Material
    G4Material* Air, * Al, * Ge, * SiO2, * Li, * Cu, * Mylar, * Kapton, * Vacuum;

    G4VisAttributes* Steel_Color, * Vacuum_Color, * Tungsten_Color;

    G4bool check_overlap = true;

private:
    G4LogicalVolume* fScoringVolume;
};
#endif

