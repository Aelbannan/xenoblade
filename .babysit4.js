const fs=require('fs');
const d=require('./.targets_status.json');
const byId={};
for(const x of d.targets) byId[x.id]=x;
for(let i=1;i<=8;i++){
  const f='.scratch/midband_prompt_0'+i+'.txt';
  if(!fs.existsSync(f)){console.log('batch 0'+i+': no prompt');continue;}
  const txt=fs.readFileSync(f,'utf8');
  const ids=[...txt.matchAll(/us-[0-9a-f]{8}/g)].map(m=>m[0]);
  const uniq=[...new Set(ids)];
  let notDone=0, done=0, missing=0;
  for(const id of uniq){
    const t=byId[id];
    if(!t){missing++;continue;}
    if(t.match_status==='FULL_MATCH'&&t.instruction_match===100) done++;
    else notDone++;
  }
  console.log('batch 0'+i+': assigned='+uniq.length+' fully-done='+done+' remaining='+notDone+' missing='+missing);
}
