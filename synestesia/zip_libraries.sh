if [ -d "libraries" ]
then
	echo "Removing existing libraries ..."
	rm -rf libraries/*
else
	echo "Directory libraries not found. Creating it ..."
	mkdir libraries
fi

if ! [ -x "$(command -v zip)" ]
then
	echo "Could not found zip command. Installing it ..."
	sudo apt-get install zip
fi

echo "Zip main libraries"
for lib in main/*
do
	if [ ! -f $lib ]
	then
		echo "* Zip $(basename $lib)"
		zip -j libraries/"$(basename $lib).zip" $lib/*
	fi
done

if [ -d "module" ]
then
	echo "Zip module libraries"
	for lib in module/*
	do
		if [ ! -f "$lib" ]
		then
			echo "* Zip $(basename $lib)" 
			zip -j libraries/"$(basename $lib).zip" $lib/*
		fi
	done
fi

echo "Libraries have been updated!"