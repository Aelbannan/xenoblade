const d=require('./.targets_status.json');
const t=d.targets;
const byUnit={};
for(const x of t){
  const u=x.unit;
  if(!byUnit[u]) byUnit[u]={total:0,matched:0};
  byUnit[u].total++;
  if(x.match_status==='FULL_MATCH'||x.match_status==='EQUIVALENT_MATCH') byUnit[u].matched++;
}
const interest=['kyoshin/CUIWindowManager','kyoshin/cf/CtrlMoveEne','kyoshin/CItemBoxLine','RVL_SDK/src/revolution/wud/WUD','monolib/src/scn/UnkClass_8047E110','monolib/src/core/code_804DEDA8','monolib/src/coli/code_804B59C8','monolib/src/scn/CScnEnvLgtCtrl','monolib/src/coli/code_804A6C60','monolib/src/effect/code_804CC2B8','kyoshin/cf/CfCamEvent_1','kyoshin/CUICfManager','kyoshin/cf/CtrlAct','kyoshin/cf/CVision','kyoshin/cf/CPartsChange','kyoshin/action/CActParamData','kyoshin/cf/CfGameManager','kyoshin/cf/CtrlObjectParam','kyoshin/code_800B06A4','kyoshin/action/CActParamAnim','kyoshin/CItemBoxGrid','kyoshin/cf/CfCam','kyoshin/code_80135FDC','kyoshin/cf/CItem','kyoshin/CEquipItemBox','kyoshin/cf/CBattleManager','kyoshin/makecrystal/CModelDispMakeCrystal','kyoshin/CFloorMap','kyoshin/CArtsInfo','kyoshin/CItemBoxInfo'];
for(const u of interest){const s=byUnit[u]; if(s) console.log(u, 'unmatched='+(s.total-s.matched)+'/'+s.total); else console.log(u,'NOT FOUND');}
// find rvl-data-2-ish units
console.log('--- units matching rvl-data:');
Object.keys(byUnit).filter(u=>/rvl.?data/i.test(u)).forEach(u=>{const s=byUnit[u];console.log(u,'unmatched='+(s.total-s.matched)+'/'+s.total)});
