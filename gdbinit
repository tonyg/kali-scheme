#
# Commands useful for debugging the Scheme48 VM.
#

#Set a breakpoint at label "raise".
#Obtain the proper line number using "egrep -n raise: scheme48vm.c".
break scheme48vm.c:5227

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
echo \040
output *(long *)((*(long *)(($cont & ~3) + 8) & ~3) + 4) / 4
echo \n
set $cont = *(long *)($cont & ~3)
preview-loop
end
#
document preview
Display Scheme stack trace.  Look up the template uids in the .debug file.
end


define show-header
echo Header length:\ 
output $hdr >> 8
echo \ type:\040
output ($hdr & 127) >> 2
echo \ tag:\040
output $hdr & 3
echo \n
end

define look
output ($ - Snewspace_beginS)
echo :\n
set $hdr = *(long *)($ - 7)
show-header
output *(long *)($ - 3)
echo \n
output *(long *)($ + 1)
echo \n
output *(long *)($ + 5)
echo \n
end

define go0
print *(long *)($ - 3)
end

define bytes
set $foo = RScode_pointerS
output (int)*(unsigned char *)($foo + 0)
echo \040
output (int)*(unsigned char *)($foo + 1)
echo \040
output (int)*(unsigned char *)($foo + 2)
echo \040
output (int)*(unsigned char *)($foo + 3)
echo \040
output (int)*(unsigned char *)($foo + 4)
echo \n
end
