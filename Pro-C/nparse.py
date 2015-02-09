import json
flink=open('2Pythonlink')
fnode=open('2Pythonnode')
fbjs=open('2Pythonbjs')
fout=open('nout.json',"w")
lim=15
njs=-1
ndic={}
nxjs={}
nbjs={}
ans={"nodes":[],"links":[]}
for line in fnode:
	word=line.strip().split()
	njs=njs+1
	ndic[int(word[0])]=njs
	nxjs[int(word[0])]=0
	ans["nodes"].append({"name":word[0]+'_'+word[1],"group":int(word[2])})
for line in fbjs:
	word=line.strip().split()
	nbjs[word[0]]=int(word[1])
for i in ans["nodes"]:
	i["degree"]=nbjs[i["name"].split('_')[0]]
for line in flink:
	word=line.strip().split()
	s=int(word[0])
	t=int(word[1])
	if (ndic[s]>lim) and (ndic[t]>lim):
		continue;
	if (ndic[s]<=lim) and (ndic[t]<=lim):
		continue;
	if (ndic[s]<=lim):
		nxjs[t]=nxjs[t]+1
	if (ndic[t]<=lim):
		nxjs[s]=nxjs[s]+1	
flink.close()
flink=open('2Pythonlink')
for line in flink:
	word=line.strip().split()
	s=int(word[0])
	t=int(word[1])
	if (nxjs[t]==0) and (nxjs[s]==0):
		continue
	ans["links"].append({"source":ndic[s],"target":ndic[t],"value":int(word[2]),"xors":(ndic[s]>lim) and (ndic[t]>lim)})
for i in ans["nodes"]:
	i["xdegree"]=nxjs[int(i["name"].split('_')[0])]
encodedjson = json.dumps(ans)
print(encodedjson,file=fout)


fout.close()


   # {"source":8,"target":0,"value":2},