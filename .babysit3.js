const d=require('./.targets_status.json');
const t=d.targets;
function show(label,pred){const m=t.filter(pred);const unm=m.filter(x=>x.match_status!=='FULL_MATCH'&&x.match_status!=='EQUIVALENT_MATCH');console.log(label+': total='+m.length+' unmatched='+unm.length);
m.slice(0,3).forEach(x=>console.log('   sample:',x.id,x.function,x.unit,x.match_status));
unm.slice(0,4).forEach(x=>console.log('   UNM:',x.id,x.function,x.unit,x.match_status));}
show('unit ~804BD8E8',x=>x.unit.toLowerCase().includes('804bd8e8'));
show('id/fn ~8017FD44',x=>(x.id+x.function+x.unit).toLowerCase().includes('8017fd44'));
show('CItem_init',x=>x.function.includes('CItem_init'));
