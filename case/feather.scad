
hole = 2.4;
rounding = 2;
$fn = 200;


width = 23-(rounding*2);
height = 51-(rounding*2);
depth = 5;

module pegs() {
  // TODO: move in more

  // top left
  translate([0, 0, depth])
    cube([hole/2, hole, 4], center = true);

  translate([0, 0, depth])
  rotate([0, 0, 90])
    cube([hole/2, hole, 4], center = true);

  // top right  
  translate([0, width, depth])
    cube([hole/2, hole, 4], center = true);

  translate([0, width, depth])
  rotate([0, 0, 90])
    cube([hole/2, hole, 4], center = true);
  

  // bottom left
  translate([height, 0, depth])
    cube([hole/2, hole, 4], center = true);

  translate([height, 0, depth])
  rotate([0, 0, 90])
    cube([hole/2, hole, 4], center = true);


  // bottom right
  translate([height, width, depth])
    cube([hole/2, hole, 4], center = true);
  
  translate([height, width, depth])
  rotate([0, 0, 90])
    cube([hole/2, hole, 4], center = true);
}

module board() {
  // top edge
  translate([0, 0, 0])
    cylinder(r = rounding, h = depth);

  translate([0, width, 0])
    cylinder(r = rounding, h = depth);

  // connect the top
  translate([(-1 * rounding), 0, 0])
    cube([rounding*2, width, depth]);

  // bottom edge
  translate([height, 0, 0])
    cylinder(r = rounding, h = depth);

  translate([height, width, 0])
    cylinder(r = rounding, h = depth);

  // connect the bottom
  translate([height - rounding, 0, 0])
    cube([rounding*2, width, depth]);


  // inner area, connect top/bottom
  translate([0, -1 * rounding, 0])
    cube([height, width+(rounding*2), depth]);
}


module feather_holder() {
  difference() {
    translate([-height/4, -width/2, -depth])
      cube([height*1.5, width*2, depth*2]);  
    
    translate([0,0,.1])
      board();
  }
}

module mouse_ears() {
  translate([-height/4, -width/2, -depth])
  linear_extrude(height=0.3)
    circle(r = 10);
  
  translate([-height/4, (-width/2)+(width*2), -depth])
  linear_extrude(height=0.3)
    circle(r = 10);
    
  translate([(-height/4)+(height*1.5), -width/2, -depth])
  linear_extrude(height=0.3)
    circle(r = 10);
  
  translate([(-height/4)+(height*1.5), (-width/2)+(width*2), -depth])
  linear_extrude(height=0.3)
    circle(r = 10);
}

/*
// holes (probably doesn't work)
translate([0, 0, 0])
  cylinder(d = hole, h = 5);

translate([0, width, 0])
  cylinder(d = hole, h = 5);

translate([height, 0, 0])
  cylinder(d = hole, h = 5);

translate([height, width, 0])
  cylinder(d = hole, h = 5);
*/


feather_holder();
mouse_ears();
