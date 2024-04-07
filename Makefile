variantDirs=$(shell find variants -type d -maxdepth 1  -not  -name "variants" -print)
buildBase=ascon-build-base.py
buildFile=ascon-build.py
dependencies=aead.c permutations.h

dstFiles=$(addsuffix  /$(buildFile), $(variantDirs))



.SECONDARYEXPANSION:
.PHONY: clean, help, print, pre

default : $(dstFiles)

$(dstFiles):  %/$(buildFile) : $$(wildcard $(dir $$@)) 
	@echo $(dir $@)
	@echo $@
	@echo $^
	@echo $<

pre:
	mkdir -p  ./out
	cp -r variants/* out/
		


clean : 
	rm -rf out/

print :
	@echo "variantDirs"
	@echo $(variantDirs)
	
	@echo "dest files"
	
	@echo $(dstFiles)

