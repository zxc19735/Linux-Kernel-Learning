savedcmd_my_chardev.mod := printf '%s\n'   my_chardev.o | awk '!x[$$0]++ { print("./"$$0) }' > my_chardev.mod
