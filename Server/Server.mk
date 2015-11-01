##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=Server
ConfigurationName      :=Debug
WorkspacePath          := "/home/aviad/git/SocketsProject"
ProjectPath            := "/home/aviad/git/SocketsProject/Server"
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=
Date                   :=01/26/15
CodeLitePath           :="/home/aviad/.codelite"
LinkerName             :=/usr/bin/g++ 
SharedObjectLinkerName :=/usr/bin/g++ -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.i
DebugSwitch            :=-g 
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
OutputFile             :=$(IntermediateDirectory)/$(ProjectName)
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="Server.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). 
IncludePCH             := 
RcIncludePath          := 
Libs                   := 
ArLibs                 :=  
LibPath                := $(LibraryPathSwitch). 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := /usr/bin/ar rcu
CXX      := /usr/bin/g++ 
CC       := /usr/bin/gcc 
CXXFLAGS :=  -g -O0 -Wall $(Preprocessors)
CFLAGS   :=  -g -O0 -Wall --std=gnu99 $(Preprocessors)
ASFLAGS  := 
AS       := /usr/bin/as 


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/main.c$(ObjectSuffix) $(IntermediateDirectory)/portAlloc.c$(ObjectSuffix) $(IntermediateDirectory)/Protocol.c$(ObjectSuffix) $(IntermediateDirectory)/binder.c$(ObjectSuffix) $(IntermediateDirectory)/HashTable.c$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild
all: $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d $(Objects) 
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

$(IntermediateDirectory)/.d:
	@test -d ./Debug || $(MakeDirCommand) ./Debug

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/main.c$(ObjectSuffix): main.c $(IntermediateDirectory)/main.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/aviad/git/SocketsProject/Server/main.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/main.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/main.c$(DependSuffix): main.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/main.c$(ObjectSuffix) -MF$(IntermediateDirectory)/main.c$(DependSuffix) -MM "main.c"

$(IntermediateDirectory)/main.c$(PreprocessSuffix): main.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/main.c$(PreprocessSuffix) "main.c"

$(IntermediateDirectory)/portAlloc.c$(ObjectSuffix): portAlloc.c $(IntermediateDirectory)/portAlloc.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/aviad/git/SocketsProject/Server/portAlloc.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/portAlloc.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/portAlloc.c$(DependSuffix): portAlloc.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/portAlloc.c$(ObjectSuffix) -MF$(IntermediateDirectory)/portAlloc.c$(DependSuffix) -MM "portAlloc.c"

$(IntermediateDirectory)/portAlloc.c$(PreprocessSuffix): portAlloc.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/portAlloc.c$(PreprocessSuffix) "portAlloc.c"

$(IntermediateDirectory)/Protocol.c$(ObjectSuffix): Protocol.c $(IntermediateDirectory)/Protocol.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/aviad/git/SocketsProject/Server/Protocol.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Protocol.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Protocol.c$(DependSuffix): Protocol.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Protocol.c$(ObjectSuffix) -MF$(IntermediateDirectory)/Protocol.c$(DependSuffix) -MM "Protocol.c"

$(IntermediateDirectory)/Protocol.c$(PreprocessSuffix): Protocol.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Protocol.c$(PreprocessSuffix) "Protocol.c"

$(IntermediateDirectory)/binder.c$(ObjectSuffix): binder.c $(IntermediateDirectory)/binder.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/aviad/git/SocketsProject/Server/binder.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/binder.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/binder.c$(DependSuffix): binder.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/binder.c$(ObjectSuffix) -MF$(IntermediateDirectory)/binder.c$(DependSuffix) -MM "binder.c"

$(IntermediateDirectory)/binder.c$(PreprocessSuffix): binder.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/binder.c$(PreprocessSuffix) "binder.c"

$(IntermediateDirectory)/HashTable.c$(ObjectSuffix): HashTable.c $(IntermediateDirectory)/HashTable.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/aviad/git/SocketsProject/Server/HashTable.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/HashTable.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/HashTable.c$(DependSuffix): HashTable.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/HashTable.c$(ObjectSuffix) -MF$(IntermediateDirectory)/HashTable.c$(DependSuffix) -MM "HashTable.c"

$(IntermediateDirectory)/HashTable.c$(PreprocessSuffix): HashTable.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/HashTable.c$(PreprocessSuffix) "HashTable.c"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) ./Debug/*$(ObjectSuffix)
	$(RM) ./Debug/*$(DependSuffix)
	$(RM) $(OutputFile)
	$(RM) "../.build-debug/Server"


