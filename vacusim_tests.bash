#	Student name: Steven Balagtas
#	Student number: n9998250
#	Unit: CAB202 Semester 1, 2019

#	This file contains bash script for the test suite used to test the Robot Vacuum simulator.


#!/bin/bash

test_num=0
REPORT_FILE=vacusim_test_report.txt

echo "Remember to do these two commands before running this script:
export COLUMNS
export LINES
"
echo "Test run starting at $(date)" >>${REPORT_FILE}

#==============================================================================
#	Operations available in the VacuSim 1.0 simulation.
#==============================================================================
function drop_dust () {
	printf "d${1},${2} " 
}

function drop_slime () {
	printf "s${1},${2} " 
}

function drop_trash () {
	printf "t${1},${2} " 
}

function up () {
	printf "i " 
}

function down () {
	printf "k " 
}

function left () {
	printf "j " 
}

function right () {
	printf "l " 
}

function move_vacuum () {
	printf "v${1},${2},${3} " 
}

function pause_resume () {
	printf "p " 
}

function reset () {
	printf "r${1},${2},${3} " 
}

function quit () {
	printf "q${1} " 
}

function set_timeout () {
	printf "o${1} " 
}

function set_delay () {
	printf "m${1} " 
}

function return_to_base () {
	printf "b " 
}

function change_load () {
	printf "w${1} " 
}

function change_battery () {
	printf "y${1} " 
}

function display_help () {
	printf "? " 
}

#========================================================================
#	Run a single test.
#========================================================================	

function run_test () {
	(( test_num ++ ))
	echo "Test    : ${test_num}"
	echo "Category: ${category}"
	echo "Details : ${details}"
	echo "Expect  : ${expect}"
	
	read -p "Press r to run the test or s to skip..." run_or_skip
	
	if [ "${run_or_skip}" == "r" ]
	then
		echo ${cmd} | ./a1_n9998250
		read -p "Did the test pass or fail (p/f)? " result
		echo "Test ${test_num}: ${result}" >>${REPORT_FILE}
	fi
}

#==============================================================================
category="Setup vacuum"
details="Vacuum alone in the center of the room, no rubbish visible."
expect="Vacuum displayed in the center, no rubbish is visible, heading displays 90, battery displays 100, elapsed time displays 00:05 before terminating, load displays 0 and rubbish status displays 0, 0, 0."

cmd=$(
	echo "0 0 0"
	set_timeout 5
)

run_test 
#==============================================================================
category="Setup dust"
details="Vacuum alone in the center of the room, 10 dust visible, no other rubbish visible, no dust overlaps any objects."
expect="Vacuum displayed in the center, 10 dust visible, no other rubbish is visible, heading displays 90, battery displays 100, elapsed time displays 00:05 before terminating, load displays 0 and rubbish status displays 10, 0, 0."

cmd=$(
	echo "10 0 0"
	set_timeout 5
)

run_test
#==============================================================================
category="Setup dust"
details="Vacuum alone in the center of the room, 100 dust visible, no other rubbish visible, no dust overlaps any objects."
expect="Vacuum displayed in the center, 100 dust visible, no other rubbish is visible, heading displays 90, battery displays 100, elapsed time displays 00:05 before terminating, load displays 0 and rubbish status displays 100, 0, 0."

cmd=$(
	echo "100 0 0"
	set_timeout 5
)

run_test
#==============================================================================
category="Setup dust"
details="Vacuum alone in the center of the room, 1000 dust visible, no other rubbish visible, no dust overlaps any objects."
expect="Vacuum displayed in the center, 1000 dust visible, no other rubbish is visible, heading displays 90, battery displays 100, elapsed time displays 00:05 before terminating, load displays 0 and rubbish status displays 1000, 0, 0."

cmd=$(
	echo "1000 0 0"
	set_timeout 5
)

run_test
#==============================================================================
category="Setup slime"
details="Vacuum alone in the center of the room, 1 slime visible, no other rubbish visible, no slime overlaps any objects."
expect="Vacuum displayed in the center, 1 slime visible, no other rubbish is visible, heading displays 90, battery displays 100, elapsed time displays 00:05 before terminating, load displays 0 and rubbish status displays 0, 1, 0."

cmd=$(
	echo "0 1 0"
	set_timeout 5
)

run_test
#==============================================================================
category="Setup slime"
details="Vacuum alone in the center of the room, 10 slime visible, no other rubbish visible, no slime overlaps any objects."
expect="Vacuum displayed in the center, 10 slime visible, no other rubbish is visible, heading displays 90, battery displays 100, elapsed time displays 00:05 before terminating, load displays 0 and rubbish status displays 0, 10, 0."

cmd=$(
	echo "0 10 0"
	set_timeout 5
)

run_test
#==============================================================================
category="Setup trash"
details="Vacuum alone in the center of the room, 1 trash visible, no other rubbish visible, no trash overlaps any objects."
expect="Vacuum displayed in the center, 1 trash visible, no other rubbish is visible, heading displays 90, battery displays 100, elapsed time displays 00:05 before terminating, load displays 0 and rubbish status displays 0, 0, 1."

cmd=$(
	echo "0 0 1"
	set_timeout 5
)

run_test
#==============================================================================
category="Setup trash"
details="Vacuum alone in the center of the room, 5 trash visible, no other rubbish visible, no trash overlaps any objects."
expect="Vacuum displayed in the center, 5 trash visible, no other rubbish is visible, heading displays 90, battery displays 100, elapsed time displays 00:05 before terminating, load displays 0 and rubbish status displays 0, 0, 5."

cmd=$(
	echo "0 0 5"
	set_timeout 5
)

run_test
#==============================================================================
category="Setup all rubbish"
details="Vacuum alone in the center of the room, 10 dust, 1 slime, 1 trash visible, no rubbish overlaps any objects."
expect="Vacuum displayed in the center, 10 dust, 1 slime, 1 trash visible, heading displays 90, battery displays 100, elapsed time displays 00:05 before terminating, load displays 0 and rubbish status displays 10, 1, 1."

cmd=$(
	echo "10 1 1"
	set_timeout 5
)

run_test
#==============================================================================
category="Setup all rubbish"
details="Vacuum alone in the center of the room, 1000 dust, 10 slime, 5 trash visible, no rubbish overlaps any objects."
expect="Vacuum displayed in the center, 1000 dust, 10 slime, 5 trash visible, heading displays 90, battery displays 100, elapsed time displays 00:05 before terminating, load displays 0 and rubbish status displays 1000, 10, 5."

cmd=$(
	echo "1000 1000 1000"
	set_timeout 5
)

run_test
#==============================================================================
category="Vacuum manual movement"
details="Vacuum alone in the center, no rubbish visible, vacuum moves all the way to the north wall and stops as it collides with the charging station."
expect="Vacuum displayed in the center, moves north one unit at a time to the charging station, stops when collides with the charging station, program terminates after 5 seconds."

cmd=$(
	echo "0 0 0"
	set_timeout 5
	for (( i = 0; i < COLUMNS; i++ )) ; do up ; done
)

run_test
#==============================================================================
category="Vacuum manual movement"
details="Vacuum alone in the center, no rubbish visible, vacuum moves all the way to the south wall and stops as it collides with the wall."
expect="Vacuum displayed in the center, moves south one unit at a time to the wall, stops when collides with the south wall, program terminates after 5 seconds."

cmd=$(
	echo "0 0 0"
	set_timeout 5
	for (( i = 0; i < COLUMNS; i++ )) ; do down ; done
)

run_test
#==============================================================================
category="Vacuum manual movement"
details="Vacuum alone in the center, no rubbish visible, vacuum moves all the way to the west wall and stops as it collides with the wall."
expect="Vacuum displayed in the center, moves west one unit at a time to the wall, stops when collides with the west wall, program terminates after 5 seconds."

cmd=$(
	echo "0 0 0"
	set_timeout 5
	for (( i = 0; i < COLUMNS; i++ )) ; do left ; done
)

run_test
#==============================================================================
category="Vacuum manual movement"
details="Vacuum alone in the center, no rubbish visible, vacuum moves all the way to the east wall and stops as it collides with the wall."
expect="Vacuum displayed in the center, moves east one unit at a time to the wall, stops when collides with the east wall, program terminates after 5 seconds."

cmd=$(
	echo "0 0 0"
	set_timeout 5
	for (( i = 0; i < COLUMNS; i++ )) ; do right ; done
)

run_test
#==============================================================================
category="Vacuum default behaviour"
details="Vacuum alone in the center, no rubbish visible, vacuum moves all the way to the south wall and changes direction after it collides with the wall."
expect="Vacuum displayed in the center, moves south one unit at a time to the wall, changes direction after colliding with the wall, heading displays 90 until vacuum changes direction, program terminates after 5 seconds."

cmd=$(
	echo "0 0 0"
	set_timeout 5
	pause_resume
)

run_test
#==============================================================================
category="Vacuum battery life"
details="Vacuum alone in the center, no rubbish visible, vacuum moves with default behaviour, battery decreases by 1% every second."
expect="Vacuum displayed in the center, moves with default behaviour, battery displays 100 then decreases by 1 every second, program terminates after 5 seconds, battery displays 95 before terminating."

cmd=$(
	echo "0 0 0"
	set_timeout 5
	pause_resume
)

run_test
#==============================================================================
category="Return to base"
details="Vacuum alone in the center, no rubbish visible, vacuum moves towards charging station and docks, this test doesn't use the triggers."
expect="Vacuum displayed in the center, battery displays 50%, vacuum moves toward charging station, docks and recharges battery then returns to default behaviour, program terminates after 5 seconds."

cmd=$(
	echo "0 0 0"
	set_timeout 5
	change_battery 50
	return_to_base
	pause_resume
)

run_test
#==============================================================================
category="Return to base"
details="Vacuum alone in the center, no rubbish visible, vacuum moves towards charging station and docks, this test uses load trigger."
expect="Vacuum displayed in the center, load displays 46, vacuum moves toward charging station, docks and unloads payload then returns to default behaviour, program terminates after 5 seconds."

cmd=$(
	echo "0 0 0"
	set_timeout 5
	change_load 46
	pause_resume
)

run_test
#==============================================================================
category="Return to base"
details="Vacuum alone in the center, no rubbish visible, vacuum moves towards charging station and docks, this test uses battery trigger."
expect="Vacuum displayed in the center, battery displays 24%, vacuum moves toward charging station, docks and recharges battery then returns to default behaviour, program terminates after 5 seconds."

cmd=$(
	echo "0 0 0"
	set_timeout 5
	change_battery 24
	pause_resume
)

run_test
#==============================================================================
category="Vacuum hack"
details="Vacuum alone in the center of the room but instantly moved to a new position and a set a new heading, no rubbish visible."
expect="Vacuum moved to top left corner, and heading set to 45 (south east), vacuum moves with default behaviour towards the south east, program terminates after 5 seconds."

cmd=$(
	echo "0 0 0"
	set_timeout 5
	move_vacuum 5 9 45
	pause_resume
)

run_test 
#==============================================================================
category="Dust collection"
details="Vacuum alone in the center, spawn dust below vacuum, vacuum collects dust and updates status display."
expect="Vacuum displayed in the center, vacuum moves with default behaviour and collects the dust, load display updates by 1, rubbish status updates from '1, 0, 0' to '0, 0, 0', program terminates after 5 seconds."

cmd=$(
	echo "0 0 0"
	set_timeout 5
	x=$((COLUMNS/ 2))
	y=$((10 + LINES/ 2))
	drop_dust ${x} ${y}
	pause_resume
)

run_test
#==============================================================================
category="Slime collection"
details="Vacuum alone in the center, spawn slime below vacuum, vacuum collects slime and updates status display."
expect="Vacuum displayed in the center, vacuum moves with default behaviour and collects the slime, load display updates by 5, rubbish status updates from '0, 1, 0' to '0, 0, 0', program terminates after 5 seconds."

cmd=$(
	echo "0 0 0"
	set_timeout 5
	x=$((COLUMNS/ 2))
	y=$((10 + LINES/ 2))
	drop_slime ${x} ${y}
	pause_resume
)

run_test
#==============================================================================
category="Trash collection"
details="Vacuum alone in the center, spawn trash below vacuum, vacuum collects trash and updates status display."
expect="Vacuum displayed in the center, vacuum moves with default behaviour and collects the trash, load display updates by 20, rubbish status updates from '0, 0, 1' to '0, 0, 0', program terminates after 5 seconds."

cmd=$(
	echo "0 0 0"
	set_timeout 5
	x=$((COLUMNS/ 2))
	y=$((10 + LINES/ 2))
	drop_trash ${x} ${y}
	pause_resume
)

run_test
#==============================================================================
category="Rubbish collection"
details="Vacuum alone in the center, spawn max rubbish, vacuum collects rubbish and updates status display, returns to base if necessary."
expect="Vacuum displayed in the center, vacuum moves with default behaviour collecting rubbish, program terminates after 20 seconds."

cmd=$(
	echo "1000 1000 1000"
	set_timeout 20
	pause_resume
)

run_test