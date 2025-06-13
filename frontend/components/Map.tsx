import { useState } from 'react';

import MapView from 'react-native-maps';


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
      region={region}
      onRegionChange={(region) => setRegion(region)}
    />
  );
}
