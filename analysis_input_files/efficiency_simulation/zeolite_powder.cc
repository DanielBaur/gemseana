
// last edited: Daniel Baur on 14th May 2021
// This file contains the GeMSE MC sample geometry for the zeolite powder (i.e., granulate) measurement.
// Therefore the powder was filled into a plastic bag, which were placed inside the GeMSE detector cavity.
// For the corresponding efficiency simulations the cavity was assumed to be entirely filled up with the custom "ptfe_powder" material (see: "dimensions").
// Therefore the "ptfe_powder" material was defined as a composite material (made up of 100% "Teflon", adapted to match the effective density of the powder, see: "materials").









// dimensions
G4double heightCuHsg_tube_filled = zSizeSampleChamber-zPosCuShielding-zSizeCuShielding/2.+zPosEndcap+heightEndcap/2.-0.001*cm;
G4Box* SampleCavity_boxvol = new G4Box("SampleCavity_box", xSizeSampleChamber/2., ySizeSampleChamber/2., zSizeSampleChamber/2.+0.001*cm);
G4Tubs* CuHsg_tube_filled = new G4Tubs ("CuHsg_tube_filled", 0.*cm, outerRadiusEndcap, heightCuHsg_tube_filled, startAngle, spanningAngle);
G4SubtractionSolid* WholeCavityFilling = new G4SubtractionSolid("filled_cavity", SampleCavity_boxvol, CuHsg_tube_filled, 0, G4ThreeVector(0.,0.,-zSizeSampleChamber/2.));
G4double sample_cavity_vol_cm3 = (xSizeSampleChamber *ySizeSampleChamber *zSizeSampleChamber)/cm3 -(outerRadiusEndcap *outerRadiusEndcap *3.1415 *heightCuHsg_tube_filled)/cm3;
G4double sample_z_cm = 5;
G4double sample_vol_cm3 = 1000;
G4double sample_x_cm = sqrt(sample_vol_cm3/sample_z_cm);
G4double sample_y_cm = sqrt(sample_vol_cm3/sample_z_cm);
G4Box* sample_box = new G4Box("sample_box", 0.5*sample_x_cm*cm, 0.5*sample_y_cm*cm, 0.5*sample_z_cm*cm);



// materials
G4double zeolite_powder_mass_g = 500;
G4Material* zeolite_powder_mat = new G4Material("zeolite_powder", zeolite_powder_mass_g/sample_vol_cm3 * g / cm3, 2, kStateSolid);
zeolite_powder_mat->AddElement(Si, 1);
zeolite_powder_mat->AddElement(O, 4);





// logical volumes
G4LogicalVolume* zeolite_powder_log = new G4LogicalVolume(sample_box, zeolite_powder_mat, "zeolite_filling_log", 0, 0, 0);
//new G4PVPlacement(0, G4ThreeVector(xSizeSampleChamber/2, ySizeSampleChamber/2, zPosEndcap+0.5*heightEndcap+0.5*sample_z_cm*cm), zeolite_powder_log, "zeolite_filling", expHall_log, false, 0);
new G4PVPlacement(0, G4ThreeVector(0, 0, zPosEndcap+0.5*heightEndcap+0.5*sample_z_cm*cm), zeolite_powder_log, "zeolite_filling", expHall_log, false, 0);





// visualization attributes
G4VisAttributes* violet = new G4VisAttributes(G4Colour(0.5, 0.0, 1.0));
zeolite_powder_log->SetVisAttributes(violet);





// Output sample mass
const G4double zeolite_powder_mass = zeolite_powder_log->GetMass(false, false) / g;
G4cout << "\n############################" << G4endl;
G4cout << "sample mass (PTFE powder): " << zeolite_powder_mass << " g" << G4endl;
G4cout << "############################" << G4endl;





