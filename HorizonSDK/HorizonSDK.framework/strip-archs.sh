function codesign {
    echo "Code signing $1 using identity \"$EXPANDED_CODE_SIGN_IDENTITY_NAME\""
    /usr/bin/codesign --force --sign "$EXPANDED_CODE_SIGN_IDENTITY" --preserve-metadata=identifier,entitlements "$1"
}

function strip_binary {
    binary="$1"
    archs=$(lipo -info "$binary" | rev | cut -d ':' -f1 | rev)
    stripped_archs=""
    for arch in $archs; do
        if [[ "$VALID_ARCHS" != *"$arch"* ]]; then
            lipo -remove "$arch" -output "$binary" "$binary" || exit 1
            stripped_archs="$stripped_archs $arch"
        fi
    done
    echo "$stripped_archs"
}

cd "$BUILT_PRODUCTS_DIR/$FRAMEWORKS_FOLDER_PATH"

framework_folder="./HorizonSDK.framework"

# Removing this script file

if [ "$ACTION" == "install" ]; then
  echo "Removing strip-archs.sh..."
  rm -f "$framework_folder/strip-archs.sh"
fi

framework="$framework_folder/HorizonSDK"

# Strip unwanted archs and codesign

stripped_archs=$(strip_binary "$framework")

if [[ -n "$stripped_archs" ]]; then
    echo "Stripped following archs from HorizonSDK: $stripped_archs"
    if [ "$CODE_SIGNING_REQUIRED" == "YES" ]; then
        codesign "$framework"
    fi
fi
