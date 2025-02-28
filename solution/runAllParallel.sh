#!/bin/bash

########################################
############# CSCI 2951-O ##############
########################################
E_BADARGS=65

if [ $# -ne 3 ] && [ $# -ne 2 ]; then
    echo "Usage: $(basename $0) <inputFolder/> <timeLimit> [<logFile>]"
    echo "Description:"
    echo -e "\t This script makes parallel calls to ./run.sh for all the files in the given inputFolder/"
    echo -e "\t Each run is subject to the given time limit in seconds."
    echo -e "\t Last line of each run is appended to the given logFile."
    echo -e "\t If a run fails, due to the time limit or other error, the file name"
    echo -e "\t is appended to the logFile with --'s as time and result. "
    echo -e "\t If the logFile already exists, the run is aborted."
    exit $E_BADARGS
fi

# Parameters
inputFolder=$1
timeLimit=$2

mkdir -p logs
if [ $# -ne 3 ]; then
    TIMESTAMP=$(date +"%Y-%m-%d_%H-%M-%S")
    logFile="logs/$TIMESTAMP.log"
else
    logFile=$3
fi

# Append slash to the end of inputFolder if it does not have it
[[ "${inputFolder: -1}" != "/" ]] && inputFolder="$inputFolder/"

# Terminate if the log file already exists
[ -f "$logFile" ] && echo "Logfile $logFile already exists, terminating." && exit 1

# Create the log file
touch "$logFile"

process_file() {
    local fullFileName="$1"
    local timeLimit="$2"
    local logFile="$3"

    echo "Running $fullFileName"
    output=$(timeout "$timeLimit" ./run.sh "$fullFileName" 2>&1)
    returnValue="$?"

    if [[ "$returnValue" = 0 ]]; then
        echo "$output" | tail -1 >>"$logFile"
    else
        instance=$(basename "$fullFileName")
        echo "{\"Instance\": \"$instance\", \"Time\": \"--\", \"Result\": \"--\"}" >>"$logFile"
    fi
}

export -f process_file

# Run all files in parallel
find "$inputFolder" -type f | parallel -j 4 process_file {} "$timeLimit" "$logFile"

echo "Processing complete. Log saved to $logFile"
