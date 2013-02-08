read -p "Enter Name For Generated Header: " -e name
echo "Making Folder ~/lib and ~/bin"
mkdir ~/lib ~/bin
echo "Adding NAME Environment Variable in .bashrc"
echo "export NAME=$name" >> ~/.bashrc
echo "Updating HOME in .bashrc"
echo "export HOME=~/" >> ~/.bashrc
echo "Updating LD_LIBRARY_PATH in .bashrc"
echo "export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$HOME/lib:./" >> ~/.bashrc
echo "Updating CPATH in .bashrc"
echo "export CPATH=$CPATH:$HOME/lib" >> ~/.bashrc
echo "Updating PATH in .bashrc"
echo "export PATH=$PATH:$HOME/bin" >> ~/.bashrc


