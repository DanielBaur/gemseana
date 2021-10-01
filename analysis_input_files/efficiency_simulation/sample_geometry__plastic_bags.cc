
// last edited: Daniel Baur on 15th June 2021
// This file contains the GeMSE MC sample geometry for the 'plastic_bags' and 'plastic_bags_with_new_firmware' measurements.
// Therefore a shitload of plastic bags was cramped into the detector cavity.
// For the corresponding efficiency simulations the cavity was assumed to be entirely filled up with the custom "plastic_bags" material (see: "dimensions").
// Therefore the "plastic_bags" material was defined as a composite material (see: "materials").











// dimensions
G4double heightCuHsg_tube_filled = zSizeSampleChamber-zPosCuShielding-zSizeCuShielding/2.+zPosEndcap+heightEndcap/2.-0.001*cm;
G4Box* SampleCavity_boxvol = new G4Box("SampleCavity_box", xSizeSampleChamber/2., ySizeSampleChamber/2., zSizeSampleChamber/2.+0.001*cm);
G4Tubs* CuHsg_tube_filled = new G4Tubs ("CuHsg_tube_filled", 0.*cm, outerRadiusEndcap, heightCuHsg_tube_filled, startAngle, spanningAngle);
G4SubtractionSolid* WholeCavityFilling = new G4SubtractionSolid("filled_cavity", SampleCavity_boxvol, CuHsg_tube_filled, 0, G4ThreeVector(0.,0.,-zSizeSampleChamber/2.));





// materials
G4double sample_cavity_vol_cm3 = (xSizeSampleChamber *ySizeSampleChamber *zSizeSampleChamber)/cm3 -(outerRadiusEndcap *outerRadiusEndcap *3.1415 *heightCuHsg_tube_filled)/cm3;
G4double plastic_bags_mass_g = 94 *8 *6; // 94g per 15 bags, 15 bags per box, 8 boxes per display, 6 displays
G4Material* plastic_bags_mat = new G4Material("plastic_bags", plastic_bags_mass_g/sample_cavity_vol_cm3 * g / cm3, 2, kStateSolid);
plastic_bags_mat->AddElement(C, 2);
plastic_bags_mat->AddElement(H, 4);


//zeolite_powder_mat->AddElement(H,4);
//4Element* Na = nist->FindOrBuildElement("Na");
//zeolite_powder_mat->AddElement(Na, 1);
//G4Element* Al = nist->FindOrBuildElement("Al");
//zeolite_powder_mat->AddElement(Al, 1);
//zeolite_powder_mat->AddElement(Si, 2);
//zeolite_powder_mat->AddElement(O, 8);




// logical volumes
G4LogicalVolume* plastic_bags_filling_log = new G4LogicalVolume(WholeCavityFilling, plastic_bags_mat, "plastic_bags_filling_log", 0, 0, 0);
new G4PVPlacement(0, G4ThreeVector(0., 0., zPosCuShielding+zSizeCuShielding/2.-zSizeSampleChamber/2.+0.001*cm), plastic_bags_filling_log, "plastic_bags_filling", expHall_log, false, 0);





// visualization attributes
G4VisAttributes* violet = new G4VisAttributes(G4Colour(0.5, 0.0, 1.0));
plastic_bags_filling_log->SetVisAttributes(violet);





// Output sample mass
const G4double plastic_bags_mass = plastic_bags_filling_log->GetMass(false, false) / g;
G4cout << "\n############################" << G4endl;
G4cout << "sample mass (plastic bags): " << plastic_bags_mass << " g" << G4endl;
G4cout << "cavity dimensions (z of the crystal cylinder): " << heightCuHsg_tube_filled << " cm" << G4endl;
G4cout << "############################" << G4endl;



