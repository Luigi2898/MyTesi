source ../progress_bar.sh
enable_trapping
echo "Starting simulations"
P=( 2 3 4 5 10 15 20 30 40 50 60)
N=( 100 )
NA=( 1 100 1000 )
DA=( 1000 10000 100000 )

progress=0
cnt=0
tot=198000
setup_scroll_area
for n in "${N[@]}"
do
    mkdir "$n""P"
    cd "$n""P"
    for na in "${NA[@]}"
    do
        mkdir "$na""Annealing"
        cd "$na""Annealing"
        for da in "${DA[@]}"
        do
            mkdir "$da""Duration"
            cd "$da""Duration"
            for p in "${P[@]}"
            do
                mkdir "$p""Precision"
                cd "$p""Precision"
                for (( q = -50; q < 51; q++ )); do
                    i=0
                    for ((in = -10; in < 11; in++)); do
                        s=$(expr 0.02*$q | bc)
                        int=$(expr 0.1*$in | bc)
                        echo "**** S = $s I = $int P = $p AD = $da AN = $na N = $n****"
                        python3 ../../../../linear_regression.py $n 1 $s $int 0.5 $p $da $na &
                        pids[${i}]=$!
                        let i=$i+1
                        let cnt=$cnt+1
                        let progress=$cnt*100/$tot
                        draw_progress_bar $progress
                    done
                    echo "Waiting for 10 annealing to end"
                    for pid in ${pids[*]}; do
                        wait $pid
                    done
                done
                cd ..
            done
            cd ..
        done
        cd ..
    done
    cd ..
done
destroy_scroll_area
