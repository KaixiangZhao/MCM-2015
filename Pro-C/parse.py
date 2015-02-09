import json
flink=open('2Pythonlink')
fnode=open('2Pythonnode')
fbjs=open('2Pythonbjs')
fout=open('out.json',"w")
njs=-1
ndic={}
nbjs={}
ans={"nodes":[],"links":[]}
for line in fnode:
	word=line.strip().split()
	njs=njs+1
	ndic[int(word[0])]=njs
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
	ans["links"].append({"source":ndic[s],"target":ndic[t],"value":int(word[2])})
encodedjson = json.dumps(ans)
print(encodedjson,file=fout)


fout.close()


   # {"source":8,"target":0,"value":2},