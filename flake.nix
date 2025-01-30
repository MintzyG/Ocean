{
  description = "A collection of fluid simulations";
  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs";
    flake-utils.url = "github:numtide/flake-utils";
  };
  outputs = { nixpkgs, flake-utils, ... }:
    flake-utils.lib.eachDefaultSystem (system: let
      pkgs = nixpkgs.legacyPackages.${system};
    in {
      packages = {
        ROL = pkgs.stdenv.mkDerivation {
          pname = "ASCII-Pond";
          version = "1.0.0";
          src = ./.;
          buildInputs = [
            pkgs.gcc
            pkgs.gdb
            pkgs.gnumake
            pkgs.pkg-config
            pkgs.premake5
          ];
          nativeBuildInputs = [ pkgs.gnumake ];
          meta = with pkgs.lib; {
            description = "Fluid simulation made using ASCII characters and C for the console";
            license = licenses.mit;
          };
          buildPhase = ''
          '';
          installPhase = ''
          '';
        };
      };
      devShells.default = pkgs.mkShell {
        buildInputs = [
          pkgs.gcc
          pkgs.pkg-config
          pkgs.gdb
          pkgs.gnumake
          pkgs.premake5
        ];
        shellHook = ''
          export LD_LIBRARY_PATH=${pkgs.lib.makeLibraryPath [ pkgs.libglvnd ]}:$LD_LIBRARY_PATH
          echo "Welcome to the development environment!"
          USER_SHELL=$(getent passwd $USER | cut -d: -f7)
          exec $USER_SHELL
        '';
      };
    });
}
