%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Make 1 plot from the results of the SOLITON test case
% 
%  Further Information:  
%  http://www.croco-ocean.org
%  
%  This file is part of CROCOTOOLS
%
%  CROCOTOOLS is free software; you can redistribute it and/or modify
%  it under the terms of the GNU General Public License as published
%  by the Free Software Foundation; either version 2 of the License,
%  or (at your option) any later version.
%
%  CROCOTOOLS is distributed in the hope that it will be useful, but
%  WITHOUT ANY WARRANTY; without even the implied warranty of
%  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
%  GNU General Public License for more details.
%
%  You should have received a copy of the GNU General Public License
%  along with this program; if not, write to the Free Software
%  Foundation, Inc., 59 Temple Place, Suite 330, Boston,
%  MA  02111-1307  USA
%
%  Copyright (c) 2002-2006 by Pierrick Penven 
%  e-mail:Pierrick.Penven@ird.fr  
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
clear all
close all
tndx=16;
makepdf=0;
nc=netcdf('soliton_his.nc','r');
time=(nc{'scrum_time'}(tndx))/(24*3600);
x=nc{'x_rho'}(:);
y=nc{'y_rho'}(:);
zeta=squeeze(nc{'zeta'}(tndx,:,:));
z1=squeeze(nc{'zeta'}(1,:,:));
close(nc);
contourf(x,y,100*zeta,[-5:2:20])
axis image
axis([0 48 0 16])
shading flat
caxis([0 12])
colorbar('h')
hold on
contour(x,y,100*z1,'k')
hold off
title(['SOLITON - zeta [cm] - day = ',num2str(time)])
if makepdf
 export_fig -transparent -pdf soliton.pdf
end

