# --- pkgIndex.tcl file for mk 1.0
proc mk:Init { dir } {
     load [file join $dir mk.dll] mk
     # source [file join $dir mk.tcl]
     }
package ifneeded mk 1.0 [list mk:Init $dir]

