#!/usr/bin/python

outfile=open("./outfile","w")
for i in range(100):
    outfile.write(str(i+1)+"hello world\n");

outfile.close()
