#
# Commands useful for debugging the Scheme48 VM.
#

#Set a breakpoint at label "raise".
#Obtain the proper line number using "egrep -n raise: scheme48vm.c".
break scheme48vm.c:5418

display/i $pc

define pcont
echo template id = \ 
output *(long *)((*(long *)(($ & ~3) + 8) & ~3) + 4) / 4
echo \npc = \ 
output (*(long *)(($ & ~3) + 4) / 4)
echo \nparent = \ 
output *(long *)($ & ~3)
echo \nenv = \ 
output *(long *)(($ & ~3) + 12)
echo \ncount = \ 
output *(long *)(($ & ~3) - 4) >> 10
echo \n
end
#
document pcont
Print $ as a continuation.
end


define parent
print *(long *)($ & ~3)
pcont
end
#
document parent
Select parent continuation.
end


define preview
set $cont = ScontS
preview-loop
end
#
define preview-loop
output $cont
echo \ \ 
output *(long *)((*(long *)(($cont & ~3) + 8) & ~3) + 4) / 4
echo \n
set $cont = *(long *)($cont & ~3)
preview-loop
end
#
document preview
Display Scheme stack trace.  Look up the template uids in the .debug file.
end
