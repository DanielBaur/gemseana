
// last edited: Daniel Baur on 14th May 2021
// This file contains the GeMSE MC sample geometry for the activated charcoal measurement.
// Therefore the charcoal spheroids were filled into five plastic bags (5 bags, ~400g each), which were placed inside the GeMSE detector cavity.
// For the corresponding efficiency simulations the cavity was assumed to be entirely filled up with the custom "activated_charcoal" material (see: "materials").
// Therefore the "activated_charcoal" material was defined as a composite material (made up of 100% "Carbon", adapted to match the effective density of the powder, see: "materials").
// NOTE: This approach is relatively valid for the ptfe powder samples. For the charcoal this could actually improved by placing cylinders, as the charcoal does not really fill out the whole cavity volume.









// dimensions
G4double heightCuHsg_tube_filled = zSizeSampleChamber-zPosCuShielding-zSizeCuShielding/2.+zPosEndcap+heightEndcap/2.-0.001*cm;
G4Box* SampleCavity_boxvol = new G4Box("SampleCavity_box", xSizeSampleChamber/2., ySizeSampleChamber/2., zSizeSampleChamber/2.+0.001*cm);
G4Tubs* CuHsg_tube_filled = new G4Tubs ("CuHsg_tube_filled", 0.*cm, outerRadiusEndcap, heightCuHsg_tube_filled, startAngle, spanningAngle);
G4SubtractionSolid* WholeCavityFilling = new G4SubtractionSolid("filled_cavity", SampleCavity_boxvol, CuHsg_tube_filled, 0, G4ThreeVector(0.,0.,-zSizeSampleChamber/2.));





// materials
G4double sample_cavity_vol_cm3 = (xSizeSampleChamber *ySizeSampleChamber *zSizeSampleChamber)/cm3 -(outerRadiusEndcap *outerRadiusEndcap *3.1415 *heightCuHsg_tube_filled)/cm3;
G4double activated_charcoal_mass_g = 379 +419 +425 +453 +477;// weights of the individual plastic bags
G4Material* activated_charcoal_mat = new G4Material("activated_charcoal", activated_charcoal_mass_g/sample_cavity_vol_cm3 * g / cm3, 1, kStateSolid);
activated_charcoal_mat->AddMaterial(C, 1.0);





// logical volumes
G4LogicalVolume* charcoal_filling_log = new G4LogicalVolume(WholeCavityFilling, activated_charcoal_mat, "charcoal_filling_log", 0, 0, 0);
new G4PVPlacement(0, G4ThreeVector(0., 0., zPosCuShielding+zSizeCuShielding/2.-zSizeSampleChamber/2.+0.001*cm), charcoal_filling_log, "charcoal_filling", expHall_log, false, 0);





// visualization attributes
G4VisAttributes* violet = new G4VisAttributes(G4Colour(0.5, 0.0, 1.0));
charcoal_filling_log->SetVisAttributes(violet);





// Output sample mass
const G4double ptfe_powder_mass = ptfe_filling_log->GetMass(false, false) / g;
G4cout << "\n############################" << G4endl;
G4cout << "sample mass (PTFE powder): " << ptfe_powder_mass << " g" << G4endl;
G4cout << "############################" << G4endl;





