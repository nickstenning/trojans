#!/usr/bin/env ruby

# Reject first line (header)
lines = STDIN.readlines[1..-1].map { |x| x.split.map { |y| y.to_f }}

include Math

BRY = 2
ANG = 4
TRO = 7
GRE = 9

def rot_coords(x, y, angle)
  xnew =  x * cos(angle) + y * sin(angle)
  ynew = -x * sin(angle) + y * cos(angle)
  return [xnew, ynew]
end

# Calculate mean positions relative to system barycenter
t_m_x, t_m_y, g_m_x, g_m_y = lines.inject([0, 0, 0, 0]) do |(tmx, tmy, gmx, gmy), x| 
  bar_x, bar_y = rot_coords(x[BRY], x[BRY+1], x[ANG])
  t_x, t_y = rot_coords(x[TRO], x[TRO+1], x[ANG])
  g_x, g_y = rot_coords(x[GRE], x[GRE+1], x[ANG])
  
  tmx += t_x - bar_x
  tmy += t_y - bar_y
  gmx += g_x - bar_x
  gmy += g_y - bar_y
  
  [tmx, tmy, gmx, gmy]
end

t_m_x /= lines.count
t_m_y /= lines.count
g_m_x /= lines.count
g_m_y /= lines.count

t_deltas, g_deltas = lines.inject([[], []]) do |(tmx, gmx), x|
  bar_x, bar_y = rot_coords(x[BRY], x[BRY+1], x[ANG])
  t_x, t_y = rot_coords(x[TRO], x[TRO+1], x[ANG])
  g_x, g_y = rot_coords(x[GRE], x[GRE+1], x[ANG])
  
  t_x -= bar_x
  t_y -= bar_y
  g_x -= bar_x
  g_y -= bar_y
  
  tmx << sqrt((t_m_x - t_x)**2 + (t_m_y - t_y)**2)
  gmx << sqrt((g_m_x - g_x)**2 + (g_m_y - g_y)**2)
  

  [tmx, gmx]
end

t_mean = t_deltas.inject(0) { |acc, x| acc + x } / t_deltas.length
g_mean = g_deltas.inject(0) { |acc, x| acc + x } / g_deltas.length

p (t_mean + g_mean) / 2