const d=require('./.targets_status.json');
const t=d.targets;
function show(label,pred){const m=t.filter(pred);const unm=m.filter(x=>x.match_status!=='FULL_MATCH'&&x.match_status!=='EQUIVALENT_MATCH');console.log(label+': total='+m.length+' unmatched='+unm.length);unm.slice(0,4).forEach(x=>console.log('   ',x.id,x.function,x.unit,x.match_status));}
show('code_804BD8E8 loose',x=>(x.id+x.function).toLowerCase().includes('804bd8e8'));
show('PartsChange',x=>x.unit.includes('CPartsChange')||/partschange/i.test(x.function));
show('MoveBase',x=>x.unit.toLowerCase().includes('movebase')||/movebase/i.test(x.function));
show('ActParam',x=>x.unit.includes('CActParam'));
show('CVision',x=>x.unit==='kyoshin/cf/CVision');
show('rvl-data units',x=>x.unit.toLowerCase().includes('rvl-data')||x.id.includes('rvl-data-2'));
