outDir=out
buildBase=ascon-build-base.py
buildFile=ascon-build.py
testFile=ascon-test.py
PYTHON=python3
PIP=pip3
testResultFiles=$(addsuffix /testResult.txt, $(variantDirs))

.PHONY: clean help    print pre build



testall : build 
	./test.sh


$(testResultFiles): %/testResult.txt : %/$(testFile)
	@echo @^
	$(PYTHON) %/$(testFile) >> %/teStResult.txt
	
pre:
	mkdir -p  ./$(outDir)
	cp -r variants/* $(outDir)
	#$(PIP) install requirements.txt

build: pre
	$(eval variantDirs=$(shell find $(outDir) -type d -maxdepth 1  -not  -name "$(outDir)" -print))
	./build.sh
		




clean : 
	rm -rf out/

print :
	@echo "variantDirs"
	@echo $(variantDirs)
	
	@echo "dest files"
	
	@echo $(dstFiles)

