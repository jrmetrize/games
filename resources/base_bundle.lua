do
  noto_sans_url = "https://noto-website-2.storage.googleapis.com/pkgs/NotoSans-hinted.zip"
  noto_serif_url = "https://noto-website-2.storage.googleapis.com/pkgs/NotoSerif-hinted.zip"
  noto_mono_url = "https://noto-website-2.storage.googleapis.com/pkgs/NotoMono-hinted.zip"

  bundle = new_resource_bundle()

  save_resource_bundle(bundle, "base")
end
