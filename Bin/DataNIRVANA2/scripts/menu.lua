-- menu.lua

-- create main menu
function naCreateMainMenu()
	-- File
	PX2EU_MAN:Menu_Main_AddMainItem("File", (PX2_LM:V("File")))
	PX2EU_MAN:Menu_Main_AddItem("File", "NewProject", PX2_LM:V("NewProject").."\tCtrl-N", "n_NewProject()", "")
	PX2EU_MAN:Menu_Main_AddItem("File", "Open", PX2_LM:V("Open").."\tCtrl-O", "n_OpenProject()", "")
	PX2EU_MAN:Menu_Main_AddItem("File", "Save", PX2_LM:V("Save").."\tCtrl-S", "n_SaveProject()", "")
	PX2EU_MAN:Menu_Main_AddItem("File", "Close", PX2_LM:V("Close"), "n_CloseProject()", "")
	PX2EU_MAN:Menu_Main_AddItemSeparater("File")
	PX2EU_MAN:Menu_Main_AddItem("File", "NewScene", PX2_LM:V("NewScene").."\tCtrl-N", "n_NewScene()", "")
	PX2EU_MAN:Menu_Main_AddItem("File", "Open", PX2_LM:V("Open"), "n_OpenScene()", "")
	PX2EU_MAN:Menu_Main_AddItem("File", "Save", PX2_LM:V("Save"), "n_SaveScene()", "")
	PX2EU_MAN:Menu_Main_AddItem("File", "SaveAs", PX2_LM:V("SaveAs"), "n_SaveSceneAs()", "")
	PX2EU_MAN:Menu_Main_AddItem("File", "Close", PX2_LM:V("Close"), "n_CloseScene()", "")
	PX2EU_MAN:Menu_Main_AddItemSeparater("File")
	PX2EU_MAN:Menu_Main_AddItem("File", "Exit", PX2_LM:V("Exit"), "n_Exit()", "")
	
	-- Edit
	PX2EU_MAN:Menu_Main_AddMainItem("Edit", (PX2_LM:V("Edit")))
	PX2EU_MAN:Menu_Main_AddItem("Edit", "OnDo", PX2_LM:V("OnDo"), "", "")
	PX2EU_MAN:Menu_Main_AddItem("Edit", "ReDo", PX2_LM:V("ReDo"), "", "")
	PX2EU_MAN:Menu_Main_AddItemSeparater("Edit")
	PX2EU_MAN:Menu_Main_AddItem("Edit", "Copy", PX2_LM:V("Copy"), "", "")
	PX2EU_MAN:Menu_Main_AddItem("Edit", "Paste", PX2_LM:V("Paste"), "", "")
	PX2EU_MAN:Menu_Main_AddItem("Edit", "Delete", PX2_LM:V("Delete"), "", "")
	PX2EU_MAN:Menu_Main_AddItemSeparater("Edit")
	PX2EU_MAN:Menu_Main_AddItem("Edit", "Import", PX2_LM:V("Import"), "", "")
	PX2EU_MAN:Menu_Main_AddItem("Edit", "Export", PX2_LM:V("Export"), "", "")
	PX2EU_MAN:Menu_Main_AddItemSeparater("Edit")
	PX2EU_MAN:Menu_Main_AddItem("Edit", "Select", PX2_LM:V("Select"), "", "")
	PX2EU_MAN:Menu_Main_AddItem("Edit", "Translate", PX2_LM:V("Translate"), "", "")
	PX2EU_MAN:Menu_Main_AddItem("Edit", "Rotate", PX2_LM:V("Rotate"), "", "")
	PX2EU_MAN:Menu_Main_AddItem("Edit", "Scale", PX2_LM:V("Scale"), "", "")
	PX2EU_MAN:Menu_Main_AddItemSeparater("Edit")
	PX2EU_MAN:Menu_Main_AddItem("Edit", "PlayAndStop", PX2_LM:V("PlayAndStop"), "", "")
	PX2EU_MAN:Menu_Main_AddItem("Edit", "ResetPlay", PX2_LM:V("ResetPlay"), "", "")
	
	-- View
	PX2EU_MAN:Menu_Main_AddMainItem("View", (PX2_LM:V("View")))
	PX2EU_MAN:Menu_Main_AddSubItem("View", "Window", (PX2_LM:V("Window")))
	PX2EU_MAN:Menu_Main_AddItem("ViewWindow", "Project", (PX2_LM:V("Project")), "", "")
	PX2EU_MAN:Menu_Main_AddItem("ViewWindow", "Stage", (PX2_LM:V("Stage")), "", "")
	PX2EU_MAN:Menu_Main_AddItem("ViewWindow", "ResView", (PX2_LM:V("ResView")), "", "")
	PX2EU_MAN:Menu_Main_AddItem("ViewWindow", "Inspector", (PX2_LM:V("Inspector")), "", "")
	PX2EU_MAN:Menu_Main_AddItem("ViewWindow", "Console", (PX2_LM:V("Console")), "", "")
	PX2EU_MAN:Menu_Main_AddItem("ViewWindow", "PreView", (PX2_LM:V("PreView")), "", "")
	
	-- Debug
	PX2EU_MAN:Menu_Main_AddMainItem("Debug", (PX2_LM:V("Debug")))
	
	-- Tool
	PX2EU_MAN:Menu_Main_AddMainItem("Tool", (PX2_LM:V("Tool")))
	
	-- Help
	PX2EU_MAN:Menu_Main_AddMainItem("Help", (PX2_LM:V("Help")))
	PX2EU_MAN:Menu_Main_AddItem("Help", "About", (PX2_LM:V("About")), "", "")
end