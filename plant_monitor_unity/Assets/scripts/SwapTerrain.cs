
// Adapted from Serge_Billault's code on the Unity forums (https://forum.unity.com/threads/changing-terrain-texture-on-runtime.1216140/)

using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using TMPro;

public class SwapTerrain : MonoBehaviour
{
    public int layerA;
    public Texture2D sand_texture;
    public Texture2D snow_texture;
    public Material skybox_snow;
    public Material skybox_sand;
    public Material skybox_m;
    public Terrain     m_terrain;
    public Texture2D[] m_textures;

    public TextMeshProUGUI temp;
    public TextMeshProUGUI humidity;
    public TextMeshProUGUI moisture;

    private float temp_val = 20f;
    private int hum_val = 30;
    private int moist_val = 40;

    // Start is called before the first frame update
    void Start()
    {
        RestoreTextures();
    }

    // Update is called once per frame
    void Update()
    {
        if (temp.GetComponent<TextMeshProUGUI>().text != "..."){
            temp_val = float.Parse(temp.GetComponent<TextMeshProUGUI>().text);
        }
        if (humidity.GetComponent<TextMeshProUGUI>().text != "..."){
            hum_val = int.Parse(humidity.GetComponent<TextMeshProUGUI>().text);
        }
        if (moisture.GetComponent<TextMeshProUGUI>().text != "..."){
            moist_val = int.Parse(moisture.GetComponent<TextMeshProUGUI>().text);
        }
        
        if (moist_val < 40){
            SwapTexture(layerA, sand_texture);
            RenderSettings.skybox = skybox_sand;
        }
        else  if (temp_val < 20f && hum_val > 40){
            SwapTexture(layerA, snow_texture);
            RenderSettings.skybox = skybox_snow;
        }
        else {
            RestoreTextures();
            RenderSettings.skybox = skybox_m;
        }

    }

    void OnDestroy()
    {
        
    }

    public void RestoreTextures()
    {
        TerrainData    data   = ( m_terrain != null ) ? m_terrain.terrainData : null;
 
        TerrainLayer[] layers = ( data      != null ) ? data.terrainLayers    : null;
                                           
        int         nb_layers = ( layers    != null ) ? layers.Length : 0;
 
 
        for( int l = 0; l < nb_layers; ++l )
        {
            TerrainLayer layer = layers[ l ];
 
            if( layer != null ) layer.diffuseTexture = m_textures[ l ];
        }
    }

    private void SwapTexture( int layer, Texture2D tex )
    {
        TerrainData    data   = ( m_terrain != null ) ? m_terrain.terrainData : null;
 
        TerrainLayer[] layers = ( data      != null ) ? data.terrainLayers    : null;
 
        if( ( layers != null ) && ( layer < layers.Length ) )
        {
            layers[ layer ].diffuseTexture = tex;
        }
    }
 
}
