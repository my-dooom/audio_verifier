########################################################################
####################### Makefile Template ##############################
########################################################################

# Compiler settings - Can be customized.
CC = gcc
CXX = g++
CFLAGS = -std=c11 -Wall
CXXFLAGS = -std=c++17 -Wall
LDFLAGS = 

# Makefile settings - Can be customized.
APPNAME = audio_verifier
SRCDIR = src
OBJDIR = obj


############## Do not change anything from here downwards! #############
CSRC = $(wildcard $(SRCDIR)/*.c)
CPPSRC = $(wildcard $(SRCDIR)/*.cpp)
OBJ = $(CSRC:$(SRCDIR)/%.c=$(OBJDIR)/%.o) \
      $(CPPSRC:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)
DEP = $(OBJ:$(OBJDIR)/%.o=%.d)
# UNIX-based OS variables & settings
RM = rm -f
DELOBJ = $(OBJ) $(DEP) $(APPNAME)
# Windows OS variables & settings
DEL = del /Q
EXE = .exe
WDELOBJ = $(OBJ) $(DEP) $(APPNAME)$(EXE)


########################################################################
####################### Targets beginning here #########################
########################################################################

all: $(APPNAME)

# Builds the app
$(APPNAME): $(OBJ)
	$(CXX) -o $@ $^ $(LDFLAGS)


# Dependency rules for C
$(OBJDIR)/%.d: $(SRCDIR)/%.c
	@$(CC) $(CFLAGS) -MM -MT '$(OBJDIR)/$*.o' $< > $@
# Dependency rules for C++
$(OBJDIR)/%.d: $(SRCDIR)/%.cpp
	@$(CXX) $(CXXFLAGS) -MM -MT '$(OBJDIR)/$*.o' $< > $@

-include $(DEP)

# Build rule for C
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -o $@ -c $<
# Build rule for C++
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $<


################### Cleaning rules for Unix-based OS ###################
# Cleans complete project
.PHONY: clean
clean:
	$(RM) $(DELOBJ) $(DEP) $(APPNAME)

# Cleans only all files with the extension .d
.PHONY: cleandep
cleandep:
	$(RM) $(DEP)

#################### Cleaning rules for Windows OS #####################
# Cleans complete project
.PHONY: cleanw
cleanw:
	$(DEL) $(WDELOBJ) $(DEP) $(APPNAME)$(EXE)

# Cleans only all files with the extension .d
.PHONY: cleandepw
cleandepw:
	$(DEL) $(DEP)
