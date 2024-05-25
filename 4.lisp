(load "../script/liberty.lisp")
(load "../script/verilog-netlist.lisp")
(setf *search-path* 
	  '(
		"../lib/libsaed32/liberty/ccs" "../lib/libsmic55/lib/1.2v" "../lib/libinp/liberty" 
		))
(setf *target-library* 
	  '(
		"saed32hvt_tt0p85v25c.txt" "scc55nll_hs_rvt_tt_v1p2_25c_basic.txt" "isf8l_ehs_generic_core_tt1v25c.txt" 
		))
#|(map-libraries
  (path lib file)
  (format t "~A~%" (lines-match "(^|\\s+)(cell|cell_footprint|pin|direction|clear|clocked_on|next_state)(\\s+)(\\S+)"  (get-lib-cells "*" "*" "(\\s+)ff(\\s+)")))
  )|#
(setf *specify* t)
#|(setf *verilog-netlist-txt* "../lib/libsaed32/verilog/saed32nm_hvt.txt")
(if (probe-file *verilog-netlist-txt*) (exec t "/bin/rm" "-rfv" *verilog-netlist-txt*))
(read-verilog-netlist "../lib/libsaed32/verilog/saed32nm_hvt.v")
(setf *verilog-netlist-txt* "../lib/libsmic55/scc55nll_hs_rvt.txt")
(if (probe-file *verilog-netlist-txt*) (exec t "/bin/rm" "-rfv" *verilog-netlist-txt*))
(read-verilog-netlist "../lib/libsmic55/scc55nll_hs_rvt.v")
(setf *verilog-netlist-txt* "../lib/libinp/hdl/isf8l_ehs_generic_core_30.msv.lib.src.txt")
(if (probe-file *verilog-netlist-txt*) (exec t "/bin/rm" "-rfv" *verilog-netlist-txt*))
(read-verilog-netlist "../lib/libinp/hdl/isf8l_ehs_generic_core_30.msv.lib.src.v")
(setf *verilog-netlist-txt* "../dc/quicksort_mapped.txt")
(if (probe-file *verilog-netlist-txt*) (exec t "/bin/rm" "-rfv" *verilog-netlist-txt*))
(read-verilog-netlist "../dc/quicksort_mapped.v")|#
(setf *verilog-netlist-txt* "../icc/quicksort_routed.txt")
(if (probe-file *verilog-netlist-txt*) (exec t "/bin/rm" "-rfv" *verilog-netlist-txt*))
(read-verilog-netlist "../icc/quicksort_routed.v")
;(exec t "/bin/rm" "-rfv" "4.v")
;(write-verilog-netlist-1 "4.v" "quicksort")
;;(format t "~S~%" *module-list*)
;(verilog-netlist-txt-hierarchy "quicksort")
;(format t "~S~%" *verilog-netlist-txt-hierarchy*)
(exec t "/bin/rm" "-rfv" "4.v")
(write-verilog-netlist "4.v" "quicksort" :hierarchy t)
