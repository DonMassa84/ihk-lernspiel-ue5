#!/bin/bash
# Build WebGL for IHK Lernspiel
# Requires: Unreal Engine 5.4+ installed, Emscripten SDK configured

set -e

PROJECT_DIR="/home/shadowmaker/UE5_IHK_Lernspiel"
PROJECT_FILE="${PROJECT_DIR}/IHK_Lernspiel.uproject"
BUILD_DIR="${PROJECT_DIR}/Build/WebGL"
UE_EDITOR="/home/shadowmaker/Downloads/Engine/Binaries/Linux/UnrealEditor"

# Check UE installation
if [ ! -f "$UE_EDITOR" ]; then
    echo "Unreal Editor not found at $UE_EDITOR"
    echo "Set UE_EDITOR path or install UE 5.4+"
    exit 1
fi

# Create project file if not exists
if [ ! -f "$PROJECT_FILE" ]; then
    cat > "$PROJECT_FILE" << 'EOF'
{
    "FileVersion": 3,
    "EngineAssociation": "5.4",
    "Category": "",
    "Description": "IHK Lernspiel - Zielvereinbarungsfragen Trainer",
    "Modules": [
        {
            "Name": "IHK_Lernspiel",
            "Type": "Runtime",
            "LoadingPhase": "Default",
            "AdditionalDependencies": ["UMG", "DataTable"]
        }
    ],
    "Plugins": [
        {
            "Name": "WebBrowser",
            "Enabled": true
        }
    ],
    "TargetPlatforms": ["WebGL"]
}
EOF
    echo "Created $PROJECT_FILE"
fi

# Build WebGL
echo "Building WebGL..."
"$UE_EDITOR" "$PROJECT_FILE" -run=Build -targetplatform=HTML5 -configuration=Shipping -clean -outputdir="$BUILD_DIR" -unattended -nocompileeditor -nop4

if [ $? -eq 0 ]; then
    echo "Build successful!"
    echo "Output: $BUILD_DIR"
    
    # Copy to deploy folder (Netlify/GitHub Pages ready)
    DEPLOY_DIR="/home/shadowmaker/ihk-projektarbeit-frs-archiv/docs/projektportal/ihk-lernspiel"
    mkdir -p "$DEPLOY_DIR"
    cp -r "$BUILD_DIR"/* "$DEPLOY_DIR"/
    echo "Deployed to: $DEPLOY_DIR"
    echo "Add to netlify.toml: [[redirects]] from = \"/ihk-lernspiel/*\" to = \"/ihk-lernspiel/:splat\" status = 200"
else
    echo "Build failed!"
    exit 1
fi