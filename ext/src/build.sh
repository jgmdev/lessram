# USE_RP_MALLOC for rpmalloc for testing

gcc -Wall -I./rpmalloc -DUSE_C_MALLOC \
    -o test \
    rpmalloc/rpmalloc.c datastoresimple.c datastore.c test.c \
    -g && ./test