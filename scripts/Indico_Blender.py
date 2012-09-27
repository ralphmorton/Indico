import bpy
import os


def write(context, filepath):
    import math
    
    use_modifiers=True
    use_normals=True
    use_tangents=True
    use_uv_coords=True
    use_colors=True

    def rvec3d(v):
        return round(v[0], 6), round(v[1], 6), round(v[2], 6)

    def rvec2d(v):
        return round(v[0], 6), round(v[1], 6)
    
    def cross3(v1, v2):
        return ((v1[1] * v2[2]) - (v1[2] * v2[1])), ((v1[2] * v2[0]) - (v1[0] * v2[2])), ((v1[0] * v2[1]) - (v1[1] * v2[0]))

    def dot3(v1,v2):
        return v1[0]*v2[0] + v1[1]*v2[1] + v1[2]*v2[2]

    def normalise3(v):
        len = math.sqrt(dot3(v,v))
        if len == 0:
            return 0, 0, 0
        else:
            return v[0]/len, v[1]/len, v[2]/len
    
    def add3(v1, v2):
        return v1[0]+v2[0], v1[1]+v2[1], v1[2]+v2[2]
    
    def getvertex(i):
        gv = gbm_verts[i][0]
        return mesh_verts[gv].co[0], mesh_verts[gv].co[1], mesh_verts[gv].co[2]
    
    def gentangents(i1, i2, i3):
        v1 = getvertex(i1)
        v2 = getvertex(i2)
        v3 = getvertex(i3)
        w1 = gbm_verts[i1][3]
        w2 = gbm_verts[i2][3]
        w3 = gbm_verts[i3][3]
        x1 = v2[0] - v1[0]
        x2 = v3[0] - v1[0]
        y1 = v2[1] - v1[1]
        y2 = v3[1] - v1[1]
        z1 = v2[2] - v1[2]
        z2 = v3[2] - v1[2]

        s1 = w2[0] - w1[0]
        s2 = w3[0] - w1[0]
        t1 = w2[1] - w1[1]
        t2 = w3[1] - w1[1]
        
        detdenom = (s1 * t2 - s2 * t1)
        if detdenom == 0.0:
            detdenom = 1.0
        r = 1.0 / detdenom
        
        return (t2 * x1 - t1 * x2) * r, (t2 * y1 - t1 * y2) * r, (t2 * z1 - t1 * z2) * r, (s1 * x2 - s2 * x1) * r, (s1 * y2 - s2 * y1) * r, (s1 * z2 - s2 * z1) * r;

    scene = context.scene
    obj = context.object

    if not obj:
        raise Exception("ERROR: Only one object may be selected")

    file = open(filepath, "w", encoding="utf8", newline="\n")

    if scene.objects.active:
        bpy.ops.object.mode_set(mode='OBJECT')

    if use_modifiers:
        mesh = obj.to_mesh(scene, True, 'PREVIEW')
    else:
        mesh = obj.data

    if not mesh:
        raise Exception("ERROR: Selected object is not mesh")
        
    materials = mesh.materials    
    
    mesh.transform(obj.matrix_world)
    mesh.calc_normals()
        
    faceUV = (len(mesh.uv_textures) > 0)
    vertexUV = (len(mesh.sticky) > 0)
    vertexColors = len(mesh.vertex_colors) > 0
    
    if (not faceUV) and (not vertexUV):
        use_uv_coords = False
        use_tangents = False
    if not vertexColors:
        use_colors = False
        
    if faceUV:
        active_uv_layer = mesh.uv_textures.active
        if not active_uv_layer:
            use_uv_coords = False
            faceUV = None
        else:
            active_uv_layer = active_uv_layer.data

    if vertexColors:
        active_col_layer = mesh.vertex_colors.active
        if not active_col_layer:
            use_colors = False
            vertexColors = None
        else:
            active_col_layer = active_col_layer.data
            
    color = uvcoord = uvcoord_key = normal = normal_key = None
    
    mesh_verts = mesh.vertices
    gbm_verts = []
    vdict = [{} for i in range(len(mesh_verts))]
    gbm_faces = [[] for f in range(len(mesh.faces))]
    gbm_facematindices = [0 for f in range(len(mesh.faces))]
    vert_count = 0
    for i, f in enumerate(mesh.faces):
        smooth = f.use_smooth
        if not smooth:
            normal = tuple(f.normal)
            normal_key = rvec3d(normal)

        if faceUV:
            uv = active_uv_layer[i]
            uv = uv.uv1, uv.uv2, uv.uv3, uv.uv4
            
        if vertexColors:
            col = active_col_layer[i]
            col = col.color1[:], col.color2[:], col.color3[:], col.color4[:]

        f_verts = f.vertices
        v = mesh_verts[f_verts[0]]
        tangent = 0, 0, 0, 0
        
        gbm_facematindices[i] = f.material_index
        pf = gbm_faces[i]
        for j, vidx in enumerate(f_verts):
            vlast = v
            v = mesh_verts[vidx]

            if smooth:
                normal = tuple(v.normal)
                normal_key = rvec3d(normal)

            if faceUV:
                uvcoord = uv[j][0], 1.0 - uv[j][1]
                uvcoord_key = rvec2d(uvcoord)
            elif vertexUV:
                uvcoord = v.uvco[0], 1.0 - v.uvco[1]
                uvcoord_key = rvec2d(uvcoord)

            if vertexColors:
                color = col[j]

            key = normal_key, uvcoord_key, color

            vdict_local = vdict[vidx]
            pf_vidx = vdict_local.get(key)
            
            if pf_vidx is None:
                pf_vidx = vdict_local[key] = vert_count
                gbm_verts.append((vidx, normal, tangent, uvcoord, color))
                vert_count += 1

            pf.append(pf_vidx)
           
    gbm_matfaces = [[] for g in range(len(materials))]
    for i, fi in enumerate(gbm_facematindices):
        gbm_matfaces[fi].append(gbm_faces[i])
    
    tan1 = [(0,0,0) for v in range(len(gbm_verts))]
    tan2 = [(0,0,0) for v in range(len(gbm_verts))]
    tangents = [(0,0,0,0) for v in range(len(gbm_verts))]
    
    for fi, mfaces in enumerate(gbm_matfaces):
        numfaces = len(mfaces)
        for mface in mfaces:
            if len(mface) == 3:
                st = gentangents(mface[2], mface[1], mface[0])
                st1 = st[0], st[1], st[2]
                st2 = st[3], st[4], st[5]
                tan1[mface[2]] = add3(tan1[mface[2]], st1)
                tan1[mface[1]] = add3(tan1[mface[1]], st1)
                tan1[mface[0]] = add3(tan1[mface[0]], st1)
                tan2[mface[2]] = add3(tan1[mface[2]], st2)
                tan2[mface[1]] = add3(tan1[mface[1]], st2)
                tan2[mface[0]] = add3(tan1[mface[0]], st2)
            else:
                st = gentangents(mface[2], mface[1], mface[0])
                st1 = st[0], st[1], st[2]
                st2 = st[3], st[4], st[5]
                tan1[mface[2]] = add3(tan1[mface[2]], st1)
                tan1[mface[1]] = add3(tan1[mface[1]], st1)
                tan1[mface[0]] = add3(tan1[mface[0]], st1)
                tan2[mface[2]] = add3(tan1[mface[2]], st2)
                tan2[mface[1]] = add3(tan1[mface[1]], st2)
                tan2[mface[0]] = add3(tan1[mface[0]], st2)
                st = gentangents(mface[3], mface[2], mface[0])
                st1 = st[0], st[1], st[2]
                st2 = st[3], st[4], st[5]
                tan1[mface[3]] = add3(tan1[mface[3]], st1)
                tan1[mface[2]] = add3(tan1[mface[2]], st1)
                tan1[mface[0]] = add3(tan1[mface[0]], st1)
                tan2[mface[3]] = add3(tan1[mface[3]], st2)
                tan2[mface[2]] = add3(tan1[mface[2]], st2)
                tan2[mface[0]] = add3(tan1[mface[0]], st2)
    
    for i, v in enumerate(gbm_verts):
        if v[1] != None:
            n = v[1][0], v[1][1], v[1][2]
        else:
            n = 0, 0, 0
            
        t = tan1[i]
        
        ndt = dot3(n, t)
        nndt = n[0]*ndt, n[1]*ndt, n[2]*ndt
        nndt = t[0]-nndt[0], t[1]-nndt[1], t[2]-nndt[2]
        nndt = normalise3(nndt)
        
        nct = cross3(n, t)
        nctdt2 = dot3(nct, tan2[i])
        
        if nctdt2 < 0:
            tw = -1
        else:
            tw = 1
            
        pos = v[0]
        norm = v[1]
        tan = nndt[0], nndt[1], nndt[2], tw
        uv = v[3]
        col = v[4]
        gbm_verts[i] = (pos, norm, tan, uv, col)
            
    #Write
    
    file.write('pos ')
    if use_normals:
        file.write('norm ')
    if use_tangents:
        file.write('tan ')
    if use_uv_coords:
        file.write('uv ')
    if use_colors:
        file.write('col ')
    file.write('\n');
    
    file.write('%d\n' % len(gbm_verts))
    
    for i, v in enumerate(gbm_verts):
        pos = mesh_verts[v[0]].co[0], mesh_verts[v[0]].co[2], mesh_verts[v[0]].co[1]
        
        if v[1] != None:
            norm = v[1][0], v[1][2], v[1][1]
        else:
            norm = 0, 0, 0
            
        if v[2] != None:
            tan = v[2][0], v[2][2], v[2][1], v[2][3]
        else:
            tan = 0, 0, 0, 0
             
        if v[3] != None:
            uv = v[3][0], v[3][1]
        else:
            uv = 0, 0
            
        if v[4] != None:
            cl = v[4][0], v[4][1], v[4][2]
        else:
            cl = 0, 0, 0
        
        file.write('%.6f %.6f %.6f ' % pos)
        if use_normals:
            file.write('%.6f %.6f %.6f ' % norm)
        if use_tangents:
            file.write('%.6f %.6f %.6f %.6f ' % tan)
        if use_uv_coords:
            file.write('%.6f %.6f ' % uv)
        if use_colors:
            file.write('%.6f %.6f %.6f' % cl)
        file.write('\n')
            
    matgen = 0
            
    for i, mfl in enumerate(gbm_matfaces):
        file.write('mat %s\n' % materials[i].name)
        matgen = matgen + 1
        for mf in mfl:
            if len(mf) == 3:
                mfr = mf[2], mf[1], mf[0]
                file.write('%d %d %d\n' % tuple(mfr))
            else:
                mft = tuple(mf)
                mf1 = mft[2], mft[1], mft[0]
                mf2 = mft[3], mft[2], mft[0]
                file.write('%d %d %d\n' % mf1)
                file.write('%d %d %d\n' % mf2)

    file.write('-\n')
    file.close()
    print("Export %r complete" % filepath)

    if use_modifiers:
        bpy.data.meshes.remove(mesh)
        

    return {'FINISHED'}

from bpy_extras.io_utils import ExportHelper
from bpy.props import StringProperty, BoolProperty, EnumProperty


class IndicoExport(bpy.types.Operator, ExportHelper):
    bl_idname = "export.iam"
    bl_label = "Export Indico Model"
    filename_ext = ".iam"
    filter_glob = StringProperty(default="*.iam", options={'HIDDEN'})

    @classmethod
    def poll(cls, context):
        return context.active_object != None

    def execute(self, context):
        return write(context, self.filepath)

def menu_func_export(self, context):
    self.layout.operator(IndicoExport.bl_idname, text="Indico Model Export")


def register():
    bpy.utils.register_class(IndicoExport)
    bpy.types.INFO_MT_file_export.append(menu_func_export)


def unregister():
    bpy.utils.unregister_class(IndicoExport)
    bpy.types.INFO_MT_file_export.remove(menu_func_export)


if __name__ == "__main__":
    register()
