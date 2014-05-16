--function for strategy
--根据播出时间，频道id，时长
 --定义比较值
 --离线时间：播出时间减当前时间大于该值为离线任务，单位：秒
 local s_tmLimitOffLine = 1000
 --关门时间：播出时间减当前时间小于该值为关门任务，单位：秒
 local s_tmLimitClose =100
 --频道列表
 local s_tbChIds = {"ch01", "ch02"}
 --素材时长,单位秒
 local s_tClipDurLimit = 100
 
 local s_tClipTypes = {"1", "2"}
 
 -----------------------------------
 --枚举定义
 local callRt_ok = 0
 local callRt_offline = 1
 local callRt_closed = 2
 local callRt_CHIdNotMatch =3
 local callRt_ClipDurNotMatch =4
 local calRt_ClipTypeNotMatch=5
 local callRt_error = -1
 ---------------------------------
 
function StgCallFunc(pObjIn, tmPlayTimeIn, strChidIn, nClipTypeIn, tClipDurIn)
 

 print(pObjIn, tmPlayTimeIn, strChidIn, nClipTypeIn, tClipDurIn)
 --check param is valid
 if (pObjIn == nil) or (tmPlayTimeIn == nil) or(strChidIn== nil)
     or(nClipTypeIn == nil) or (tClipDurIn == nil) then
     print ("param not valid!")
     return 1;
 end
 
  bRet = callRt_ok
 --check play time
 if s_tmLimitOffLine ~= nil  and s_tmLimitClose ~= nil then
   --time in second
     tmNow = os.time()
     print("currenttime = ", tmNow)
     tmDetla = tmPlayTimeIn - tmNow

     strRunInfo = ""
     nRunInfoType = 1
     --check playtime
     if tmDetla < s_tmLimitOffLine then
      if tmDetla < s_tmLimitClose then
        --closed
        strRunInfo ="closed"
        bRet = callRt_closed
      else
        --online
        strRunInfo = "online"
        bRet = callRt_ok
      end
     else
      --offline
        strRunInfo = "offline"
        bRet = callRt_offline
     end
     
     FuncCallbackToC(nRunInfoType, strRunInfo)
   end
 
 
  if bRet ~= callRt_ok then
   return bRet
 end
 
 --check chid
 if s_tbChIds ~=nil then
    bRet = callRt_CHIdNotMatch
   for k,v in pairs(s_tbChIds) do
     if v == strChidIn then
       FuncCallbackToC(1, "chid founded")
       bRet = callRt_ok
       break
     end
   end
 end
 
if bRet ~= callRt_ok then
     FuncCallbackToC(1, "chid not founded")
   return bRet
 end
 
   --check type
 if s_tbChIds ~=nil then
    bRet = calRt_ClipTypeNotMatch
   for k,v in pairs(s_tClipTypes) do
     if v == nClipTypeIn then
       FuncCallbackToC(1, "clip type founded")
       bRet = callRt_ok
       break
     end
   end
 end
 
if bRet ~= callRt_ok then
     FuncCallbackToC(1, "clip type not founded")
   return bRet
 end
 
 --check file length
if  s_tClipDurLimit ~= nil then
  if tClipDurIn <= s_tClipDurLimit then
    FuncCallbackToC(1, "clip  dur matched ")
    bRet = callRt_ok
  else
    bRet = callRt_ClipDurNotMatch
    FuncCallbackToC(1, "clip  dur not matched")
  end
end
 
  print("returned: ", bRet)
  return bRet
end




function FuncCallbackToC(nTypeIn, strInfoIn)
  print("callback info: ", strInfoIn)
end


StgCallFunc(111, os.time() +103, "ch01", "1", 20)