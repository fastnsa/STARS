#(1) our_model.start_tau 
#(2) our_model.start_tau + our_model.incr_tau
# :
# :
#(D) our_model.start_tau + (D-1) * our_model.incr_tau

#Example for calling all methods using the Chicago dataset in the "dataset" directory with L = 4, D = 5, start_tau = 100, and incr_tau = 100:
L=5
D=4
start_tau=100
incr_tau=100
dir="./dataset/"
dataset="Chicago"

method=1 #RQS
result_file="Chicago_M"$method
./STARS $dir$dataset $result_file $method $L $D $start_tau $incr_tau

method=2 #SPS
result_file="Chicago_M"$method
./STARS $dir$dataset $result_file $method $L $D $start_tau $incr_tau

method=3 #CA
result_file="Chicago_M"$method
./STARS $dir$dataset $result_file $method $L $D $start_tau $incr_tau

method=4 #NS
result_file="Chicago_M"$method
./STARS $dir$dataset $result_file $method $L $D $start_tau $incr_tau

method=5 #CA^{(ASPS)}
result_file="Chicago_M"$method
./STARS $dir$dataset $result_file $method $L $D $start_tau $incr_tau

method=6 #NS^{(ASPS)}
result_file="Chicago_M"$method
./STARS $dir$dataset $result_file $method $L $D $start_tau $incr_tau

# ----------------------------------------------------------
eps=0.1

method=7 STARS
result_file="Chicago_M"$method
./STARS $dir$dataset $result_file $method $L $D $start_tau $incr_tau $eps

method=8 NS+SAMPLING
result_file="Chicago_M"$method
./STARS $dir$dataset $result_file $method $L $D $start_tau $incr_tau $eps