circle_count = 100;
handle_thickness = 10;
edge_radius = 2;
finger_hole_diam = 20;
string_hole_diam = 3;
string_hole_thick_factor = 1.3;

module rounded_square( width, radius_corner ) {
	translate( [ radius_corner, radius_corner, 0 ] )
		minkowski() {
			square( width - 2 * radius_corner );
			circle( radius_corner, $fn=circle_count);
		}
}

difference(){
    union(){
        for(i = [-1:2:1]){
            translate([i*(finger_hole_diam+handle_thickness/2),0,0])
                rotate_extrude(angle=180, $fn=circle_count)
                    translate([finger_hole_diam,0,0])
                        rounded_square(handle_thickness, edge_radius);
        }
        rotate_extrude(angle=-180, $fn=circle_count)
            translate([finger_hole_diam*2 + handle_thickness/2,0,0])
                rounded_square(handle_thickness, edge_radius);
 //       translate([0,-(finger_hole_diam*2 + (handle_thickness)/2 + edge_radius),handle_thickness/2])
//       rotate([90,0,0])
//            cylinder(d=handle_thickness*string_hole_thick_factor, h=(handle_thickness-2*edge_radius), $fn=circle_count);
    }
    translate([0,-(finger_hole_diam*2 + (handle_thickness)*0.9/2),handle_thickness/2])
        rotate([90,0,0])
            cylinder(d=string_hole_diam, h=handle_thickness*1.1, $fn=circle_count);
}



