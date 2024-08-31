#include "DetectorConstruction.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4GlobalMagFieldMessenger.hh"
#include "G4AutoDelete.hh"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

#include "G4MultiUnion.hh"
#include "G4SubtractionSolid.hh"

DetectorConstruction::DetectorConstruction()
	: G4VUserDetectorConstruction()
{}

DetectorConstruction::~DetectorConstruction()
{}

G4VPhysicalVolume* DetectorConstruction::Construct()
{
	// Define materials
	DefineMaterials();

	// Define volumes
	return DefineVolumes();
}

void DetectorConstruction::DefineMaterials()
{
	// NIST
	G4NistManager* nist = G4NistManager::Instance();

	// Air
	Air = nist->FindOrBuildMaterial("G4_AIR");

	// Al
	Al = nist->FindOrBuildMaterial("G4_Al");

	// Li
	Li = nist->FindOrBuildMaterial("G4_Li");

	// Ge
	Ge = nist->FindOrBuildMaterial("G4_Ge");

	// SiO2
	SiO2 = nist->FindOrBuildMaterial("G4_SILICON_DIOXIDE");

	// Cu
	Cu = nist->FindOrBuildMaterial("G4_Cu");

	// Mylar
	Mylar = nist->FindOrBuildMaterial("G4_MYLAR");

	// Kapton
	Kapton = nist->FindOrBuildMaterial("G4_KAPTON");

	// Vacuum
	Vacuum = nist->FindOrBuildMaterial("G4_Galactic");

	Vacuum_Color = new G4VisAttributes(G4Colour(1., 0., 0., 0.1));
	Vacuum_Color->SetVisibility(true);
	Vacuum_Color->SetForceSolid(true);

	// Print materials
	G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}

G4VPhysicalVolume* DetectorConstruction::DefineVolumes()
{
	// ------------------------------------------------------------------------------------------
	// Mother world volume
	// ------------------------------------------------------------------------------------------
	solidWorld = new G4Box("solidWorld", 50 * cm, 50 * cm, 50 * cm);

	logicWorld = new G4LogicalVolume(solidWorld, Air, "logicWorld");

	physWorld = new G4PVPlacement(0, G4ThreeVector(), logicWorld, "physWorld", 0, false, 0, check_overlap);

	logicWorld->SetVisAttributes(G4VisAttributes::GetInvisible());

	// ------------------------------------------------------------------------------------------
	// Endcap
	// ------------------------------------------------------------------------------------------
	G4RotationMatrix* Rotation1 = new G4RotationMatrix();
	Rotation1->rotateY(0. * deg);
	Rotation1->rotateX(90. * deg);
	Rotation1->rotateZ(0. * deg);

	solid_Endcap_Outer = new G4Tubs("solid_Endcap_Outer", 0., 3.81 * cm, 6.452 * cm, 0 * deg, 360 * deg);
	solid_Endcap_Inner = new G4Tubs("solid_Endcap_Inner", 0., 3.66 * cm, 6.377 * cm, 0 * deg, 360 * deg);

	G4RotationMatrix rotm = G4RotationMatrix();
	G4ThreeVector position1 = G4ThreeVector(0., 0., 0.);
	G4Transform3D tr1 = G4Transform3D(rotm, position1);

	G4ThreeVector position2 = G4ThreeVector(0., 0., -0.075 * cm);
	G4Transform3D tr2 = G4Transform3D(rotm, position2);
	G4SubtractionSolid* solid_Endcap = new G4SubtractionSolid("solid_Endcap", solid_Endcap_Outer, solid_Endcap_Inner, tr2);
	
	logic_Endcap = new G4LogicalVolume(solid_Endcap, Al, "logic_Endcap_Outer");
	new G4PVPlacement(Rotation1, G4ThreeVector(0., -6.452 *cm, 0.), logic_Endcap, "logic_Endcap", logicWorld, false, 0, check_overlap);

	// ------------------------------------------------------------------------------------------
	// Vacuum volume
	// ------------------------------------------------------------------------------------------

	logic_Vacuum_Volume = new G4LogicalVolume(solid_Endcap_Inner, Vacuum, "logic_Vacuum_Volume");
	new G4PVPlacement(Rotation1, G4ThreeVector(0., -6.527 * cm, 0.), logic_Vacuum_Volume, "phys_Vacuum_Volume", logicWorld, false, 0, check_overlap);

	// ------------------------------------------------------------------------------------------
	// Germanium Crystal
	// ------------------------------------------------------------------------------------------
	solid_Ge = new G4Tubs("solid_Ge", 0., 2.9 * cm, 2.65 * cm, 0. * deg, 360 * deg);
	solid_Electrode_Hole = new G4Tubs("solid_Electrode_Hole", 0., 0.3751 * cm, 1.651 * cm, 0. * deg, 360 * deg);
	G4ThreeVector position3 = G4ThreeVector(0., 0., -1.001 * cm);
	G4Transform3D tr3 = G4Transform3D(rotm, position3);

	G4SubtractionSolid* solid_Ge_Crystal = new G4SubtractionSolid("solid_Ge_Crystal", solid_Ge, solid_Electrode_Hole, tr3);

	logic_Ge_Crystal = new G4LogicalVolume(solid_Ge_Crystal, Ge, "logic_Ge_Crystal");
	new G4PVPlacement(0, G4ThreeVector(0.,0., 3.183 * cm), logic_Ge_Crystal, "phys_Ge_Crystal", logic_Vacuum_Volume, false, 0, check_overlap);
	
	// Set Scoring Volume
	fScoringVolume = logic_Ge_Crystal;

	// ------------------------------------------------------------------------------------------
	// Window Electrode
	// ------------------------------------------------------------------------------------------
	solid_Electrode_Hole = new G4Tubs("solid_Electrode_Hole", 0., 2.9 * cm, 0.022 * cm, 0. * deg, 360 * deg);
	logic_Window_Electrode = new G4LogicalVolume(solid_Electrode_Hole, SiO2, "logic_Window_Electrode");
	new G4PVPlacement(0, G4ThreeVector(0., 0., 5.855 * cm), logic_Window_Electrode, "phys_Window_Electrode", logic_Vacuum_Volume, false, 0, check_overlap);

	// ------------------------------------------------------------------------------------------
	// Outer Electrode
	// ------------------------------------------------------------------------------------------
	solid_Outer_Electrode_P1 = new G4Tubs("solid_Outer_Electrode_P1", 0., 2.944 * cm, 2.672 * cm, 0. * deg, 360 * deg);
	solid_Outer_Electrode_P2 = new G4Tubs("solid_Outer_Electrode_P2", 0., 2.9 * cm, 2.672 * cm, 0. * deg, 360 * deg);
	G4SubtractionSolid* solid_Outer_Electrode = new G4SubtractionSolid("solid_Outer_Electrode", solid_Outer_Electrode_P1, solid_Outer_Electrode_P2);
	logic_Outer_Electrode = new G4LogicalVolume(solid_Outer_Electrode, Ge, "logic_Outer_Electrode");
	new G4PVPlacement(0, G4ThreeVector(0., 0., 3.205 * cm), logic_Outer_Electrode, "phys_Outer_Electrode", logic_Vacuum_Volume, false, 0, check_overlap);

	// ------------------------------------------------------------------------------------------
	// Crystal Holder
	// ------------------------------------------------------------------------------------------
	solid_Crystal_Holder_P1 = new G4Tubs("solid_Crystal_Holder_P1", 2.944 * cm, 3.214 * cm, 0.1615 * cm, 0. * deg, 360 * deg);

	solid_Crystal_Holder_P2 = new G4Tubs("solid_Crystal_Holder_P2", 2.944 * cm, 3.02 * cm, 0.5735 * cm, 0. * deg, 360 * deg);
	G4ThreeVector position4 = G4ThreeVector(0., 0., -0.735 * cm);
	G4Transform3D tr4 = G4Transform3D(rotm, position4);

	solid_Crystal_Holder_P3 = new G4Tubs("solid_Crystal_Holder_P3", 2.944 * cm, 3.214 * cm, 0.43 * cm, 0. * deg, 360 * deg);
	G4ThreeVector position5 = G4ThreeVector(0., 0., -1.7385 * cm);
	G4Transform3D tr5 = G4Transform3D(rotm, position5);

	solid_Crystal_Holder_P4 = new G4Tubs("solid_Crystal_Holder_P4", 2.944 * cm, 3.02 * cm, 0.315 * cm, 0. * deg, 360 * deg);
	G4ThreeVector position6 = G4ThreeVector(0., 0., -2.4835 * cm);
	G4Transform3D tr6 = G4Transform3D(rotm, position6);

	solid_Crystal_Holder_P5 = new G4Tubs("solid_Crystal_Holder_P5", 2.944 * cm, 3.214 * cm, 0.43 * cm, 0. * deg, 360 * deg);
	G4ThreeVector position7 = G4ThreeVector(0., 0., -3.2285 * cm);
	G4Transform3D tr7 = G4Transform3D(rotm, position7);

	solid_Crystal_Holder_P6 = new G4Tubs("solid_Crystal_Holder_P6", 2.944 * cm, 3.02 * cm, 1.592 * cm, 0. * deg, 360 * deg);
	G4ThreeVector position8 = G4ThreeVector(0., 0., -5.2505 * cm);
	G4Transform3D tr8 = G4Transform3D(rotm, position8);

	solid_Crystal_Holder_P7 = new G4Tubs("solid_Crystal_Holder_P7", 0.75 * cm, 3.02 * cm, 0.16 * cm, 0. * deg, 360 * deg);
	G4ThreeVector position9 = G4ThreeVector(0., 0., -7.0025 * cm);
	G4Transform3D tr9 = G4Transform3D(rotm, position9);

	solid_Crystal_Holder_P8 = new G4Tubs("solid_Crystal_Holder_P8", 0.75 * cm, 1.22 * cm, 0.8245 * cm, 0. * deg, 360 * deg);
	G4ThreeVector position10 = G4ThreeVector(0., 0., -7.987 * cm);
	G4Transform3D tr10 = G4Transform3D(rotm, position10);

	solid_Crystal_Holder_P9 = new G4Tubs("solid_Crystal_Holder_P9", 0.125 * cm, 1.22 * cm, 0.4055 * cm, 0. * deg, 360 * deg);
	G4ThreeVector position11 = G4ThreeVector(0., 0., -9.217 * cm);
	G4Transform3D tr11 = G4Transform3D(rotm, position11);

	G4MultiUnion* munion_solid = new G4MultiUnion("munion_solid");
	munion_solid->AddNode(*solid_Crystal_Holder_P1, tr1);
	munion_solid->AddNode(*solid_Crystal_Holder_P2, tr4);
	munion_solid->AddNode(*solid_Crystal_Holder_P3, tr5);
	munion_solid->AddNode(*solid_Crystal_Holder_P4, tr6);
	munion_solid->AddNode(*solid_Crystal_Holder_P5, tr7);
	munion_solid->AddNode(*solid_Crystal_Holder_P6, tr8);
	munion_solid->AddNode(*solid_Crystal_Holder_P7, tr9);
	munion_solid->AddNode(*solid_Crystal_Holder_P8, tr10);
	munion_solid->AddNode(*solid_Crystal_Holder_P9, tr11);
	munion_solid->Voxelize();

	logic_Crystal_Holder = new G4LogicalVolume(munion_solid, Al, "logic_Crystal_Holder");
	new G4PVPlacement(0, G4ThreeVector(0., 0., 5.7155 * cm), logic_Crystal_Holder, "phys_Crystal_Holder", logic_Vacuum_Volume, false, 0, check_overlap);
	
	// ------------------------------------------------------------------------------------------
	// Teflon Insulator
	// ------------------------------------------------------------------------------------------
	solid_Teflon_Insulator_P1 = new G4Tubs("solid_Teflon_Insulator_P1", 0.165 * cm, 0.75 * cm, 0.7595 * cm, 0. * deg, 360 * deg);
	
	solid_Teflon_Insulator_P2 = new G4Tubs("solid_Teflon_Insulator_P2", 0. * cm, 0.75 * cm, 0.225 * cm, 0. * deg, 360 * deg);
	G4ThreeVector position12 = G4ThreeVector(0., 0., -0.9845 * cm);
	G4Transform3D tr12 = G4Transform3D(rotm, position12);

	G4MultiUnion* solid_Teflon_Insulator = new G4MultiUnion("solid_Teflon_Insulator");
	solid_Teflon_Insulator->AddNode(*solid_Teflon_Insulator_P1, tr1);
	solid_Teflon_Insulator->AddNode(*solid_Teflon_Insulator_P2, tr12);
	solid_Teflon_Insulator->Voxelize();

	logic_Teflon_Insulator = new G4LogicalVolume(solid_Teflon_Insulator, Li, "logic_Teflon_Insulator");
	new G4PVPlacement(0, G4ThreeVector(0., 0., -1.8865 * cm), logic_Teflon_Insulator, "phys_Teflon_Insulator", logic_Vacuum_Volume, false, 0, check_overlap);

	// ------------------------------------------------------------------------------------------
	// Signal Core
	// ------------------------------------------------------------------------------------------
	solid_Signal_Core = new G4Tubs("solid_Signal_Core", 0., 0.165 * cm, 3.065 * cm, 0. * deg, 360 * deg);
	logic_Signal_Core = new G4LogicalVolume(solid_Signal_Core, Cu, "logic_Signal_Core");
	new G4PVPlacement(0, G4ThreeVector(0., 0., 0.419 * cm), logic_Signal_Core, "phys_Signal_Core", logic_Vacuum_Volume, false, 0, check_overlap);

	return physWorld;
}

void DetectorConstruction::ConstructSDandField()
{}
