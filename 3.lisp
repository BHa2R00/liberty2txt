(load "../script/cdl.lisp")
(setf *cdl-txt* "../lib/libsaed32/cdl/saed32nm_hvt.txt")
(if (probe-file *cdl-txt*) (exec t "/bin/rm" "-rfv" *cdl-txt*))
(read-cdl "../lib/libsaed32/cdl/saed32nm_hvt.sp")
;(format t "~S~%" *subckts*)
(setf *cdl-txt* "../lib/libsmic55/scc55nll_hs_rvt.txt")
(if (probe-file *cdl-txt*) (exec t "/bin/rm" "-rfv" *cdl-txt*))
(read-cdl "../lib/libsmic55/scc55nll_hs_rvt.cdl")
;(format t "~S~%" *subckts*)
(setf *cdl-txt* "../lib/libinp/cdl/isf8l_ehs_generic_core_mpg.msv.txt")
(if (probe-file *cdl-txt*) (exec t "/bin/rm" "-rfv" *cdl-txt*))
(read-cdl "../lib/libinp/cdl/isf8l_ehs_generic_core_mpg.msv.sp")
;(format t "~S~%" *subckts*)
