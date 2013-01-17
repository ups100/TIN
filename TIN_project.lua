TIN_project = Proto("Tin_project", "TIN_project protocol")
local f = TIN_project.fields

f.msgid = ProtoField.uint8 ("TIN_project.msgid", "Message Id")
f.size = ProtoField.uint32("TIN_project.size", "Size of data")
f.data = ProtoField.bytes("Tin_project.data", "Data")

function TIN_project.dissector (buffer, pinfo, tree)

	local subtree = tree:add(TIN_project, buffer())
	local offset = 0
	
	local msgid = buffer(offset,1):uint()
	
	subtree:add(f.msgid, msgid)
	offset = offset + 1

	pinfo.cols.protocol = "TIN_project"
	pinfo.cols.info = "MESSAGE TYPE: "
	if msgid == 0 then 
		pinfo.cols.info:append("CONNECT TO ALIAS")
		subtree:add(f.size,buffer(offset,4))
	elseif msgid == 1 then
		pinfo.cols.info:append("CONNECTED TO ALIAS")
	elseif msgid == 2 then
		pinfo.cols.info:append("NOT CONNECTED TO ALIAS")	
	elseif msgid == 3 then
		pinfo.cols.info:append("CREATE ALIAS")
		subtree:add(f.size,buffer(offset,4))
	elseif msgid == 4 then
		pinfo.cols.info:append("ALIAS CREATED")	
	elseif msgid == 5 then
		pinfo.cols.info:append("ALIAS NOT CREATED")
	elseif msgid == 6 then
		pinfo.cols.info:append("REMOVE ALIAS")
		subtree:add(f.size,buffer(offset,4))	
	elseif msgid == 7 then
		pinfo.cols.info:append("ALIAS REMOVED")
	elseif msgid == 8 then
		pinfo.cols.info:append("ALIAS NOT REMOVED")
	elseif msgid == 9 then
		pinfo.cols.info:append("LIST ALIAS")
	elseif msgid == 10 then
		pinfo.cols.info:append("ALIAS LISTED")
		subtree:add(f.size,buffer(offset,4))
	elseif msgid == 11 then
		pinfo.cols.info:append("FIND FILE")
		subtree:add(f.size,buffer(offset,4))	
	elseif msgid == 12 then
		pinfo.cols.info:append("FILE LOCATION")
		subtree:add(f.size,buffer(offset,4))	
	elseif msgid == 13 then
		pinfo.cols.info:append("FILE NOT FOUND")
	elseif msgid == 14 then
		pinfo.cols.info:append("PULL FILE")
		subtree:add(f.size,buffer(offset,4))
	elseif msgid == 15 then
		pinfo.cols.info:append("NO SUCH FILE")
	elseif msgid == 16 then
		pinfo.cols.info:append("TRANSFER IN PROGRESS")
	elseif msgid == 17 then
		pinfo.cols.info:append("TRANSFER FINISHED")
	elseif msgid == 18 then
		pinfo.cols.info:append("TRANSFER ERROR")
	elseif msgid == 19 then
		pinfo.cols.info:append("DELETE FROM ALIAS")
		subtree:add(f.size,buffer(offset,4))
	elseif msgid == 20 then
		pinfo.cols.info:append("DELETED FROM ALIAS")
	elseif msgid == 21 then
		pinfo.cols.info:append("NOT DELETED FROM ALIAS")
	elseif msgid == 22 then
		pinfo.cols.info:append("PUSH FILE")
		subtree:add(f.size,buffer(offset,4))
	elseif msgid == 23 then
		pinfo.cols.info:append("RECEIVE FILE")
		subtree:add(f.size,buffer(offset,4))
	elseif msgid == 24 then
		pinfo.cols.info:append("SEND FILE")
		subtree:add(f.size,buffer(offset,4))
	elseif msgid == 25 then
		pinfo.cols.info:append("LIST YOUR FILES")
	elseif msgid == 26 then
		pinfo.cols.info:append("FILES LIST")
		subtree:add(f.size,buffer(offset,4))
	elseif msgid == 27 then
		pinfo.cols.info:append("DELETE YOUR FILE")
		subtree:add(f.size,buffer(offset,4))	
	elseif msgid == 28 then
		pinfo.cols.info:append("FIND YOUR FILE")
		subtree:add(f.size,buffer(offset,4))	
	elseif msgid == 29 then
		pinfo.cols.info:append("NO SUCH FILE HERE")
	elseif msgid == 30 then
		pinfo.cols.info:append("FILE FOUND HERE")
		subtree:add(f.size,buffer(offset,4))
	elseif msgid == 31 then
		pinfo.cols.info:append("ADD DIRECTORY")
		subtree:add(f.size,buffer(offset,4))
	elseif msgid == 32 then
		pinfo.cols.info:append("ADD DIRECTORY AND CONNECT")
		subtree:add(f.size,buffer(offset,4))
	elseif msgid == 33 then
		pinfo.cols.info:append("REMOVE DIRECTORY AND DISCONNECT")
		subtree:add(f.size,buffer(offset,4))
	else pinfo.cols.info:append("PACKAGE NOT RECOGNIZED")
	end

end

function TIN_project.init()
end

tcp_table = DissectorTable.get("tcp.port")
tcp_table:add(8888,TIN_project)
