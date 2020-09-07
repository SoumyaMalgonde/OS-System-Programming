#!/bin/sh

# Author : Soumya Malgonde
# Aim : Shell scripting
# 	Write a program to implement student database with options given below:
# 	1) Create student database.
# 	2) View student database.
# 	3) Insert a new student's record.
# 	4) Search a record.
# 	5) Delete a record.
# 	6) Modify a record.

# (Compilation using: bash filename.sh)

loop=1
filename=""
while [ $loop -eq 1 ];
do
echo "-------------------------------------------------------------------"
echo -e "MENU\n
1. Create student database\n2. View student database.\n3. Insert a new student's record.\n4. Search a record.\n5. Delete a record.\n6. Modify a record.\n7. Exit Menu.\n Enter your option: \c"
read choice
case "$choice" in
1)
	# create database
	echo "Enter name of file to be created: "
	read filename
	if [ -z $filename ];
		then echo -e "You have not entered file name.\n"
	elif [ -f $filename ];
	then echo -e "File already exists in system!\n"
	else touch $filename
		echo -e "Student database created in file : $filename\n"
	fi
	;;
2)	
	# view database
	if [ -z $filename ];
	then
		echo -e "There is no database in the system.\n"
	else
		echo -e "Student database : \n"
		echo -e "Roll no. Name Maths Science CS Average Result"
		cat $filename
	fi
	;;
3) 
	# insert a record
	if [ -z $filename ];
	then
		echo -e "There is no database in the system.\n"
	else
		echo -e "Enter details in order(roll no., name, marks in sub1, sub2, sub3 respectively):\n"
		read record

		rollno=$(echo $record| cut -d " " -f 1)
		m1=$(echo $record| cut -d " " -f 3)
		m2=$(echo $record| cut -d " " -f 4)
		m3=$(echo $record| cut -d " " -f 5)
		sum=$(expr "$m1" + "$m2" + "$m3")
		avg=$(expr $sum / 3)

		class=""
		if [ $avg -ge 75 ];
		then
			class="Distinction"
		elif [ $avg -ge 60 ];
		then
			class="First division"
		elif [ $avg -ge 45 ];
		then
			class="Second division"
		elif [ $avg -ge 35 ];
		then
			class="Pass"
		else
			class="Fail"
		fi

		var=$(grep "$rollno" $filename)
		if [ $? -eq 0 ];
		then echo -e "Record already exists.\n"
		else
			echo $record $avg $class >> $filename
			echo -e "Record inserted successfully.\n"
		fi
	fi
	;;
4)  
	# search a record
	if [ -z $filename ];
	then
		echo -e "There is no database in the system.\n"
	else
		echo "Enter roll number of the student whose record has to be searched :"
		read rollno
		var=$(grep "$rollno" $filename)
		if [ $? -eq 0 ];
		then 
			echo -e "Student's details are as follows:\n"
			echo $var
		else
			echo "Record not found."
		fi
	fi
	;;	
5)  
	# delete a record
	if [ -z $filename ];
	then
		echo -e "There is no database in the system.\n"
	else
		echo "Enter roll number of the student whose record has to be deleted :"
		read roll
		var=$(grep "$rollno" $filename)
		if [ $? -eq 0 ];
		then
			grep -v $roll $filename>>tmep.txt
			mv tmep.txt $filename
			echo "Record deleted successfully."
		else
			echo "Record not present."
		fi
	fi
	;;
6)
	# modify a record
	if [ -z $filename ];
	then
		echo -e "There is no database in the system.\n"
	else
		echo "Enter roll number of the student whose record has to be modified :"
		read rollno
		record=$(grep "$rollno" $filename)
		if [ $? -eq 0 ];
		then
			rollno=$(echo $record| cut -d " " -f 1)
			name=$(echo $record| cut -d " " -f 2)
			m1=$(echo $record| cut -d " " -f 3)
			m2=$(echo $record| cut -d " " -f 4)
			m3=$(echo $record| cut -d " " -f 5)
			avg=$(echo $record| cut -d " " -f 6)
			class=$(echo $record| cut -d " " -f 7)
			echo "Details are : $record"
			modloop=1
			while [ $modloop -eq 1 ];
			do
				echo -e "What field is to be modified:"
				echo -e "1. Name\n2. Marks in subject 1\n3. Marks in subject 2\n4. Marks in subject 3.\n5. Exit\n Enter choice:"
				read modchoice
				case "$modchoice" in
				1) 
					echo "Enter name :"
					read name
					;;
				2) 
					echo "Enter marks in subject 1 :"
					read m1
					;;
				3) 
					echo "Enter marks in subject 2 :"
					read m2
					;;
				4) 
					echo "Enter marks in subject 3 :"
					read m3
					;;
				5) modloop=0;;
				*) echo -e "Please enter a valid option.\n"
				esac
			done
			sum=$(expr "$m1" + "$m2" + "$m3")
			avg=$(expr $sum / 3)
			if [ $avg -ge 75 ];
			then
				class="Distinction"
			elif [ $avg -ge 60 ];
			then
				class="First division"
			elif [ $avg -ge 45 ];
			then
				class="Second division"
			elif [ $avg -ge 35 ];
			then
				class="Pass"
			else
				class="Fail"
			fi
			sed "s/$record/$rollno $name $m1 $m2 $m3 $avg $class/" $filename >> tmep.txt
			mv tmep.txt $filename
			echo "Record modified successfully."
		else
			echo "Record to be modified is not present."
		fi
	fi
	;;
7) 
	# exit menu
	loop=0;;
*) echo -e "Please enter a valid option.\n"
esac

done