
clc
clear

_r_base         = csvread( "results_pso.csv" );

_r_bat          = csvread( "results_bat.csv" );
_r_cuckoo       = csvread( "results_cuckoo.csv" );
_r_differential = csvread( "results_differential.csv" );
_r_flower       = csvread( "results_flower.csv" );
_r_genocop      = csvread( "results_genocop.csv" );
_r_gravitation  = csvread( "results_gravitation.csv" );
_r_harmonic     = csvread( "results_harmonic.csv" );
_r_wolf         = csvread( "results_wolf.csv" );

_r_others = _r_bat;
_r_others(:,:,2) = _r_cuckoo;
_r_others(:,:,3) = _r_differential;
_r_others(:,:,4) = _r_flower;
_r_others(:,:,5) = _r_genocop;
_r_others(:,:,6) = _r_gravitation;
_r_others(:,:,7) = _r_harmonic;
_r_others(:,:,8) = _r_wolf;

_other_meta = [1,2,3,4,5,6,7,8];
_test_fields = [1,2,3,4,5,6];
_test_names = { "ackley2d", "ackley8d", "schwefel2d", "schwefel8d", "schaffer2d", "schaffer8d" };
_meta_names = { "bat", "cuckoo", "differential", "flower", "genocop", "gravitation", "harmonic", "wolf" };

for i = [1,2,3,4,5,6,7,8]

  for field = [1,2,3,4,5,6]
  
    x = _r_base(:,field);
    y = _r_others(:,field,i);
    
    if sum( x != y ) < 25
      pval = 0;
    else
      [pval, z] = wilcoxon_test(x,y);
    endif
    printf( "wt_(%s)_(%s) = %d\n\r", _test_names{field}, _meta_names{i}, pval )
    
  endfor

endfor

%[pval, z] = wilcoxon_test(x,y)
