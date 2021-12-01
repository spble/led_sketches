circle_count=360;

difference(){
    cylinder(120, d1=60, d2=32, $fn=circle_count);
    translate([0,0,120-45]){
        cylinder(h=45, d=28, $fn=circle_count);
    }
    translate([0,0,120-45-58]){
        cylinder(h=58, d1=19, d2=22, $fn=circle_count);
    }
    

    translate([7,-7.5,120-45-58]){
        cube([10,15,120]);
    }


    translate([10,-7.5,120-30]){
        cube([10,15,120]);
    }
}