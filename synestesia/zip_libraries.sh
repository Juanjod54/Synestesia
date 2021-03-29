if [ -d "libraries" ]
then
	echo "Removing existing libraries ...\n"
	rm -rf libraries/*
else
	echo "Directory libraries not found. Creating it ...\n"
	mkdir libraries
fi

if ! [ -x "$(command -v zip)" ]
then
	echo "Could not found zip command. Installing it ...\n"
	sudo apt-get install zip
fi

for lib in main/*
do
	echo "$(basename $lib)" 
	zip -j libraries/"$(basename $lib).zip" $lib/*
done

echo "\nLibraries have been updated!\n"
