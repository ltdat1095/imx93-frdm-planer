# Publish Mender sdimg artifacts with .wic naming for tooling compatibility.
#
# This avoids building the non-Mender .wic image while still providing:
#   <image>.wic      -> <image>.sdimg
#   <image>.wic.zst  -> <image>.sdimg.zst

mender_create_wic_aliases() {
    case " ${IMAGE_FSTYPES} " in
        *" sdimg "*|*" sdimg.zst "*)
            ;;
        *)
            return
            ;;
    esac

    for base in "${IMAGE_NAME}" "${IMAGE_LINK_NAME}"; do
        sdimg="${IMGDEPLOYDIR}/${base}.sdimg"
        sdimg_zst="${IMGDEPLOYDIR}/${base}.sdimg.zst"
        wic="${IMGDEPLOYDIR}/${base}.wic"
        wic_zst="${IMGDEPLOYDIR}/${base}.wic.zst"

        if [ -e "${sdimg}" ]; then
            ln -sfn "$(basename "${sdimg}")" "${wic}"
            bbnote "Created WIC alias: ${wic} -> ${sdimg}"
        fi

        if [ -e "${sdimg_zst}" ]; then
            ln -sfn "$(basename "${sdimg_zst}")" "${wic_zst}"
            bbnote "Created WIC alias: ${wic_zst} -> ${sdimg_zst}"
        fi
    done
}

do_image_complete[postfuncs] += "mender_create_wic_aliases"
