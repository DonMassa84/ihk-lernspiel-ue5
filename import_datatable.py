#!/usr/bin/env python3
"""
Import CSV DataTable into Unreal Engine 5
Usage: Run inside Unreal Engine Python Editor (Script Editor) or via commandlet
"""

import unreal
import csv
import os

# Configuration
PROJECT_PATH = "/Game/Data"
CSV_PATH = "/home/shadowmaker/UE5_IHK_Lernspiel/Content/Data/DT_Questions.csv"
DATATABLE_NAME = "DT_Questions"
STRUCT_NAME = "QuestionRowStruct"  # Must exist in project

def create_struct_if_needed():
    """Create the struct asset if it doesn't exist"""
    struct_path = f"/Game/Data/{STRUCT_NAME}"
    if not unreal.EditorAssetLibrary.does_asset_exist(struct_path):
        # Create struct via Python (requires UE 5.1+)
        struct_factory = unreal.StructFactory()
        struct_asset = unreal.EditorAssetLibrary.create_asset(
            struct_path,
            unreal.Struct,
            struct_factory,
            unreal.AssetCreateContext()
        )
        # Add properties
        props = [
            ("QuestionID", "Name"),
            ("Priority", "Int32"),
            ("Category", "String"),
            ("QuestionText", "String"),
            ("ModelAnswer", "String"),
            ("Keywords", "String"),
            ("Difficulty", "Int32"),
        ]
        for prop_name, prop_type in props:
            unreal.EditorUtilityLibrary.add_struct_property(struct_asset, prop_name, prop_type)
        unreal.EditorAssetLibrary.save_asset(struct_path)
        print(f"Created struct: {struct_path}")
    else:
        print(f"Struct exists: {struct_path}")

def import_csv_to_datatable():
    """Import CSV as DataTable"""
    # Ensure struct exists
    create_struct_if_needed()
    
    # Load struct
    struct_path = f"/Game/Data/{STRUCT_NAME}"
    row_struct = unreal.EditorAssetLibrary.load_asset(struct_path)
    
    # Create DataTable factory
    dt_factory = unreal.DataTableFactory()
    dt_factory.row_struct = row_struct
    
    # Import
    import_task = unreal.AssetImportTask()
    import_task.automated = True
    import_task.destination_path = PROJECT_PATH
    import_task.destination_name = DATATABLE_NAME
    import_task.factory = dt_factory
    import_task.filename = CSV_PATH
    import_task.replace_existing = True
    import_task.save = True
    
    result = unreal.AssetToolsHelpers.get_asset_tools().import_asset_tasks([import_task])
    
    if result and result[0].imported_object:
        dt = result[0].imported_object
        print(f"DataTable imported: {dt.get_path_name()}")
        print(f"Rows: {dt.get_row_names()}")
    else:
        print("Import failed")

def verify_datatable():
    """Verify the imported DataTable"""
    dt_path = f"{PROJECT_PATH}/{DATATABLE_NAME}"
    if unreal.EditorAssetLibrary.does_asset_exist(dt_path):
        dt = unreal.EditorAssetLibrary.load_asset(dt_path)
        row_names = dt.get_row_names()
        print(f"\nVerification: {len(row_names)} rows")
        for name in row_names[:3]:  # Show first 3
            row = dt.find_row(name, "")
            print(f"  {name}: {row.QuestionText[:60]}...")
    else:
        print("DataTable not found")

if __name__ == "__main__":
    import_csv_to_datatable()
    verify_datatable()