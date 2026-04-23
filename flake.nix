{
  description = "cisastor - C/C++ library collection";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = nixpkgs.legacyPackages.${system};
      in {
        devShells.default = pkgs.mkShell.override { stdenv = pkgs.clangStdenv; } {
          packages = with pkgs; [
            clang
            clang-tools  # clangd, clang-format, clang-tidy
            cmake
            ninja
            pkg-config
			git
          ];

          shellHook = ''
            export CC=clang
            export CXX=clang++
            rm -f compile_commands.json
            cmake -B build -G Ninja -DCMAKE_EXPORT_COMPILE_COMMANDS=ON > /dev/null 2>&1 && \
              ln -sf build/compile_commands.json compile_commands.json
            echo "cisastor dev shell ready"
          '';
        };
      });
}
