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

echo "Zip main libraries"
for lib in main/*
do
	echo "$(basename $lib)" 
	zip -j libraries/"$(basename $lib).zip" $lib/*
done

if [ -d "module" ]
	echo "Zip module libraries"
	for lib in module/*
	do
		echo "$(basename $lib)" 
		zip -j libraries/"$(basename $lib).zip" $lib/*
	done
fi

echo "\nLibraries have been updated!\n"
