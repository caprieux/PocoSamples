if [ -z $DEBUG ]; then
    ./$1
else
    DYLD_LIBRARY_PATH=$HOME/Work/PocoSource/poco-1.9.0/lib/Darwin/x86_64 ./$1
fi
