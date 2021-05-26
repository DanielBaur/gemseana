
// last edited: Daniel Baur on 14th May 2021
// This file contains the GeMSE MC sample geometry for the PTFE powder measurements.
// Therefore the powder was filled into plastic bags (25 bags, ~286g each), which were placed inside the GeMSE detector cavity.
// For the corresponding efficiency simulations the cavity was assumed to be entirely filled up with the custom "ptfe_powder" material (see: "dimensions").
// Therefore the "ptfe_powder" material was defined as a composite material (made up of 100% "Teflon", adapted to match the effective density of the powder, see: "materials").









// dimensions
G4double heightCuHsg_tube_filled = zSizeSampleChamber-zPosCuShielding-zSizeCuShielding/2.+zPosEndcap+heightEndcap/2.-0.001*cm;
G4Box* SampleCavity_boxvol = new G4Box("SampleCavity_box", xSizeSampleChamber/2., ySizeSampleChamber/2., zSizeSampleChamber/2.+0.001*cm);
G4Tubs* CuHsg_tube_filled = new G4Tubs ("CuHsg_tube_filled", 0.*cm, outerRadiusEndcap, heightCuHsg_tube_filled, startAngle, spanningAngle);
G4SubtractionSolid* WholeCavityFilling = new G4SubtractionSolid("filled_cavity", SampleCavity_boxvol, CuHsg_tube_filled, 0, G4ThreeVector(0.,0.,-zSizeSampleChamber/2.));





// materials
G4double sample_cavity_vol_cm3 = (xSizeSampleChamber *ySizeSampleChamber *zSizeSampleChamber)/cm3 -(outerRadiusEndcap *outerRadiusEndcap *3.1415 *heightCuHsg_tube_filled)/cm3;
G4double ptfe_powder_mass_g = 25* 286;
G4Material* ptfe_powder_mat = new G4Material("ptfe_powder", ptfe_powder_mass_g/sample_cavity_vol_cm3 * g / cm3, 1, kStateSolid);
ptfe_powder_mat->AddMaterial(Teflon, 1.0);


// materials: alternative
// G4material* ptfe_powder_mat = new G4Material("ptfe_powder", ptfe_powder_mass_g/sample_cavity_vol_cm3 * g / cm3, 2, kStateSolid);
// ptfe_powder_mat->AddMaterial(Teflon, 0.5);
// ptfe_powder_mat->AddMaterial(air_mat,0.5);





// logical volumes
G4LogicalVolume* ptfe_filling_log = new G4LogicalVolume(WholeCavityFilling, ptfe_powder_mat, "ptfe_filling_log", 0, 0, 0);
new G4PVPlacement(0, G4ThreeVector(0., 0., zPosCuShielding+zSizeCuShielding/2.-zSizeSampleChamber/2.+0.001*cm), ptfe_filling_log, "ptfe_filling", expHall_log, false, 0);





// visualization attributes
G4VisAttributes* violet = new G4VisAttributes(G4Colour(0.5, 0.0, 1.0));
ptfe_filling_log->SetVisAttributes(violet);





// Output sample mass
const G4double ptfe_powder_mass = ptfe_filling_log->GetMass(false, false) / g;
G4cout << "\n############################" << G4endl;
G4cout << "sample mass (PTFE powder): " << ptfe_powder_mass << " g" << G4endl;
G4cout << "############################" << G4endl;





