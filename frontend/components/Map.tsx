import { useState } from 'react';
import { View, Text, StyleSheet } from 'react-native';

import MapView, { Marker, PROVIDER_GOOGLE } from 'react-native-maps';
import { EdgeDevice } from './Measurements';


interface Region {
    latitude: number;
    longitude: number;
    latitudeDelta: number;
    longitudeDelta: number;
}


interface MapProps {
  devices: EdgeDevice[]; 
}


const getNumberOfSecondsAgo = (timestamp: number) => {
  return Math.floor((Date.now() / 1000) - timestamp);
}


export default function Map({
  devices,
}: MapProps) {
    const [region, setRegion] = useState<Region>({
      latitude: 52.40166973,
      longitude: 16.9499582,
      latitudeDelta: 0.0922,
      longitudeDelta: 0.0421,
    });

  return (
    <MapView
      style={styles.map}
      provider={PROVIDER_GOOGLE}
      region={region}
      /*
      onRegionChange={(region) => setRegion({
        ...region
      })}
      showsMyLocationButton={false}
      showsUserLocation={false}
      showsCompass={false}
      rotateEnabled={true}
      scrollEnabled={true}
      zoomEnabled={true}
      moveOnMarkerPress={true}
      */
    >
      {
        devices.map((device) =>
          device.location &&
          device.measurements[0] &&
          <Marker
            key={device.name}
            coordinate={device.location}
            title={device.name}
            style={{
              width: 1000,
              height: 500,
              
            }}
            description={`pm10: ${device.measurements[0].Pm10Standard}  pm25: ${device.measurements[0].Pm25Standard}  pm100: ${device.measurements[0].Pm100Standard}  ${getNumberOfSecondsAgo(device.measurements[0].DeviceTimestamp)} s age`}
          >
          </Marker>
        )
      }
    </MapView>
  );
}

const styles = StyleSheet.create({
  map: {
    ...StyleSheet.absoluteFillObject,
    top: 100,
    height: 600,
  },
})