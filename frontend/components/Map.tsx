import { useState } from 'react';
import { View, Text, StyleSheet } from 'react-native';

import MapView, { PROVIDER_GOOGLE } from 'react-native-maps';


interface Region {
    latitude: number;
    longitude: number;
    latitudeDelta: number;
    longitudeDelta: number;
}


export default function Map() {
    const [region, setRegion] = useState<Region>({
      latitude: 37.78825,
      longitude: -122.4324,
      latitudeDelta: 0.0922,
      longitudeDelta: 0.0421,
    });

  return (
    <MapView
      style={styles.map}
      provider={PROVIDER_GOOGLE}
      region={region}
      onRegionChange={(region) => setRegion(region)}
    />
  );
}

const styles = StyleSheet.create({
  map: {
    ...StyleSheet.absoluteFillObject,
    top: 100,
    height: 600,
  },
})